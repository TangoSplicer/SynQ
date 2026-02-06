/**
 * WebSocket Connection Manager
 * Handles WebSocket connections with automatic reconnection, message queuing, and heartbeat
 */

export type MessageType = 'join' | 'leave' | 'edit' | 'presence' | 'comment' | 'sync' | 'ack' | 'error';

export interface CollaborationMessage {
  type: MessageType;
  sessionId: string;
  userId: string;
  timestamp: number;
  data: any;
  revision?: number;
  messageId?: string;
}

export interface WebSocketConfig {
  url: string;
  reconnectAttempts?: number;
  reconnectDelay?: number;
  maxReconnectDelay?: number;
  heartbeatInterval?: number;
  messageQueueSize?: number;
}

export class WebSocketManager {
  private ws: WebSocket | null = null;
  private url: string;
  private reconnectAttempts: number = 5;
  private reconnectDelay: number = 1000;
  private maxReconnectDelay: number = 30000;
  private heartbeatInterval: number = 30000;
  private messageQueueSize: number = 1000;
  
  private currentAttempt: number = 0;
  private messageQueue: CollaborationMessage[] = [];
  private heartbeatTimer: NodeJS.Timeout | null = null;
  private reconnectTimer: NodeJS.Timeout | null = null;
  private messageHandlers: Map<string, (message: CollaborationMessage) => void> = new Map();
  private connectionCallbacks: ((connected: boolean) => void)[] = [];
  private isConnected: boolean = false;
  private userId: string = '';
  private sessionId: string = '';

  constructor(config: WebSocketConfig) {
    this.url = config.url;
    this.reconnectAttempts = config.reconnectAttempts || 5;
    this.reconnectDelay = config.reconnectDelay || 1000;
    this.maxReconnectDelay = config.maxReconnectDelay || 30000;
    this.heartbeatInterval = config.heartbeatInterval || 30000;
    this.messageQueueSize = config.messageQueueSize || 1000;
  }

  /**
   * Connect to WebSocket server
   */
  public connect(userId: string, sessionId: string): Promise<void> {
    return new Promise((resolve, reject) => {
      try {
        this.userId = userId;
        this.sessionId = sessionId;
        
        const wsUrl = `${this.url}?userId=${userId}&sessionId=${sessionId}`;
        this.ws = new WebSocket(wsUrl);

        this.ws.onopen = () => {
          console.log('WebSocket connected');
          this.isConnected = true;
          this.currentAttempt = 0;
          this.startHeartbeat();
          this.flushMessageQueue();
          this.notifyConnectionChange(true);
          resolve();
        };

        this.ws.onmessage = (event) => {
          this.handleMessage(event.data);
        };

        this.ws.onerror = (error) => {
          console.error('WebSocket error:', error);
          reject(error);
        };

        this.ws.onclose = () => {
          console.log('WebSocket disconnected');
          this.isConnected = false;
          this.stopHeartbeat();
          this.notifyConnectionChange(false);
          this.attemptReconnect();
        };
      } catch (error) {
        reject(error);
      }
    });
  }

  /**
   * Disconnect from WebSocket server
   */
  public disconnect(): void {
    this.stopHeartbeat();
    if (this.reconnectTimer) {
      clearTimeout(this.reconnectTimer);
    }
    if (this.ws) {
      this.ws.close();
      this.ws = null;
    }
    this.isConnected = false;
    this.messageQueue = [];
  }

  /**
   * Send message to server
   */
  public sendMessage(message: CollaborationMessage): void {
    if (!this.isConnected || !this.ws) {
      this.queueMessage(message);
      return;
    }

    try {
      this.ws.send(JSON.stringify(message));
    } catch (error) {
      console.error('Failed to send message:', error);
      this.queueMessage(message);
    }
  }

  /**
   * Register message handler for specific type
   */
  public onMessage(type: MessageType, handler: (message: CollaborationMessage) => void): void {
    this.messageHandlers.set(type, handler);
  }

  /**
   * Register connection state callback
   */
  public onConnectionChange(callback: (connected: boolean) => void): void {
    this.connectionCallbacks.push(callback);
  }

  /**
   * Get connection status
   */
  public isConnectedStatus(): boolean {
    return this.isConnected;
  }

  /**
   * Get message queue size
   */
  public getQueueSize(): number {
    return this.messageQueue.length;
  }

  // Private methods

  private handleMessage(data: string): void {
    try {
      const message: CollaborationMessage = JSON.parse(data);
      const handler = this.messageHandlers.get(message.type);
      if (handler) {
        handler(message);
      }
    } catch (error) {
      console.error('Failed to parse message:', error);
    }
  }

  private queueMessage(message: CollaborationMessage): void {
    if (this.messageQueue.length >= this.messageQueueSize) {
      console.warn('Message queue full, dropping oldest message');
      this.messageQueue.shift();
    }
    this.messageQueue.push(message);
  }

  private flushMessageQueue(): void {
    while (this.messageQueue.length > 0 && this.isConnected && this.ws) {
      const message = this.messageQueue.shift();
      if (message) {
        try {
          this.ws.send(JSON.stringify(message));
        } catch (error) {
          console.error('Failed to flush queued message:', error);
          this.messageQueue.unshift(message);
          break;
        }
      }
    }
  }

  private startHeartbeat(): void {
    this.heartbeatTimer = setInterval(() => {
      if (this.isConnected && this.ws) {
        try {
          this.ws.send(JSON.stringify({
            type: 'ping',
            timestamp: Date.now(),
          }));
        } catch (error) {
          console.error('Failed to send heartbeat:', error);
        }
      }
    }, this.heartbeatInterval);
  }

  private stopHeartbeat(): void {
    if (this.heartbeatTimer) {
      clearInterval(this.heartbeatTimer);
      this.heartbeatTimer = null;
    }
  }

  private attemptReconnect(): void {
    if (this.currentAttempt >= this.reconnectAttempts) {
      console.error('Max reconnection attempts reached');
      return;
    }

    this.currentAttempt++;
    const delay = Math.min(
      this.reconnectDelay * Math.pow(2, this.currentAttempt - 1),
      this.maxReconnectDelay
    );

    console.log(`Attempting to reconnect in ${delay}ms (attempt ${this.currentAttempt}/${this.reconnectAttempts})`);

    this.reconnectTimer = setTimeout(() => {
      this.connect(this.userId, this.sessionId).catch((error) => {
        console.error('Reconnection failed:', error);
      });
    }, delay);
  }

  private notifyConnectionChange(connected: boolean): void {
    this.connectionCallbacks.forEach((callback) => {
      try {
        callback(connected);
      } catch (error) {
        console.error('Error in connection callback:', error);
      }
    });
  }
}

/**
 * Create WebSocket manager instance
 */
export function createWebSocketManager(config: WebSocketConfig): WebSocketManager {
  return new WebSocketManager(config);
}
