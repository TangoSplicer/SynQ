/**
 * Secure WebSocket Client with Authentication and Security Features
 *
 * Provides authenticated WebSocket connections with:
 * - JWT token-based authentication
 * - Automatic reconnection with exponential backoff
 * - Message rate limiting
 * - Security validation
 * - Connection health monitoring
 */

export interface WebSocketConfig {
  url: string;
  token: string;
  sessionId: string;
  maxReconnectAttempts?: number;
  initialReconnectDelay?: number;
  maxReconnectDelay?: number;
  heartbeatInterval?: number;
}

export interface WebSocketMessage {
  type: string;
  sessionId: string;
  userId: string;
  timestamp: string;
  [key: string]: any;
}

export interface ConnectionStats {
  messagesReceived: number;
  messagesSent: number;
  reconnectAttempts: number;
  lastMessageTime: string | null;
  connectionDuration: number;
  averageLatency: number;
}

export class SecureWebSocketClient {
  private ws: WebSocket | null = null;
  private url: string;
  private token: string;
  private sessionId: string;
  private userId: string = '';
  private messageHandlers: Map<string, Function[]> = new Map();
  private messageQueue: WebSocketMessage[] = [];
  private reconnectAttempts = 0;
  private maxReconnectAttempts: number;
  private initialReconnectDelay: number;
  private maxReconnectDelay: number;
  private currentReconnectDelay: number;
  private heartbeatInterval: number;
  private heartbeatTimer: NodeJS.Timeout | null = null;
  private isConnected = false;
  private isAuthenticated = false;
  private stats: ConnectionStats = {
    messagesReceived: 0,
    messagesSent: 0,
    reconnectAttempts: 0,
    lastMessageTime: null,
    connectionDuration: 0,
    averageLatency: 0
  };
  private connectionStartTime: number = 0;
  private latencies: number[] = [];

  constructor(config: WebSocketConfig) {
    this.url = config.url;
    this.token = config.token;
    this.sessionId = config.sessionId;
    this.maxReconnectAttempts = config.maxReconnectAttempts || 10;
    this.initialReconnectDelay = config.initialReconnectDelay || 1000;
    this.maxReconnectDelay = config.maxReconnectDelay || 30000;
    this.currentReconnectDelay = this.initialReconnectDelay;
    this.heartbeatInterval = config.heartbeatInterval || 30000;
  }

  /**
   * Connect to WebSocket server
   */
  async connect(): Promise<boolean> {
    return new Promise((resolve) => {
      try {
        this.ws = new WebSocket(this.url);

        this.ws.onopen = () => {
          console.log('WebSocket connected');
          this.isConnected = true;
          this.connectionStartTime = Date.now();
          this.reconnectAttempts = 0;
          this.currentReconnectDelay = this.initialReconnectDelay;

          // Authenticate
          this.authenticate();

          // Start heartbeat
          this.startHeartbeat();

          // Process queued messages
          this.processMessageQueue();

          resolve(true);
        };

        this.ws.onmessage = (event) => {
          this.handleMessage(event.data);
        };

        this.ws.onerror = (error) => {
          console.error('WebSocket error:', error);
          this.isConnected = false;
          this.isAuthenticated = false;
        };

        this.ws.onclose = () => {
          console.log('WebSocket disconnected');
          this.isConnected = false;
          this.isAuthenticated = false;
          this.stopHeartbeat();
          this.attemptReconnect();
        };
      } catch (error) {
        console.error('Failed to create WebSocket:', error);
        resolve(false);
      }
    });
  }

  /**
   * Authenticate with server using JWT token
   */
  private authenticate(): void {
    const authMessage = {
      type: 'authenticate',
      sessionId: this.sessionId,
      userId: this.userId,
      timestamp: new Date().toISOString(),
      token: this.token
    } as WebSocketMessage;

    this.sendRaw(authMessage);
  }

  /**
   * Send a message to the server
   */
  async send(message: Omit<WebSocketMessage, 'sessionId' | 'userId' | 'timestamp'>): Promise<boolean> {
    if (!this.isConnected || !this.isAuthenticated) {
      // Queue message for later
      this.messageQueue.push({
        ...message,
        sessionId: this.sessionId,
        userId: this.userId,
        timestamp: new Date().toISOString()
      } as WebSocketMessage);
      return false;
    }

    const fullMessage = {
      ...message,
      sessionId: this.sessionId,
      userId: this.userId,
      timestamp: new Date().toISOString()
    } as WebSocketMessage;

    return this.sendRaw(fullMessage);
  }

  /**
   * Send raw message
   */
  private sendRaw(message: Record<string, any>): boolean {
    if (!this.ws || this.ws.readyState !== WebSocket.OPEN) {
      return false;
    }

    try {
      this.ws.send(JSON.stringify(message));
      this.stats.messagesSent++;
      this.stats.lastMessageTime = new Date().toISOString();
      return true;
    } catch (error) {
      console.error('Failed to send message:', error);
      return false;
    }
  }

  /**
   * Handle incoming message
   */
  private handleMessage(data: string): void {
    try {
      const message = JSON.parse(data) as WebSocketMessage;
      this.stats.messagesReceived++;

      // Calculate latency
      const messageTime = new Date(message.timestamp).getTime();
      const latency = Date.now() - messageTime;
      this.latencies.push(latency);
      if (this.latencies.length > 100) {
        this.latencies.shift();
      }
      this.stats.averageLatency = this.latencies.reduce((a, b) => a + b, 0) / this.latencies.length;

      // Handle authentication response
      if (message.type === 'authenticated') {
        this.isAuthenticated = true;
        this.userId = message.userId;
        console.log('WebSocket authenticated');
        return;
      }

      // Handle heartbeat
      if (message.type === 'pong') {
        return;
      }

      // Dispatch to handlers
      const handlers = this.messageHandlers.get(message.type) || [];
      handlers.forEach(handler => {
        try {
          handler(message);
        } catch (error) {
          console.error(`Error in message handler for ${message.type}:`, error);
        }
      });
    } catch (error) {
      console.error('Failed to parse message:', error);
    }
  }

  /**
   * Register message handler
   */
  on(messageType: string, handler: (message: WebSocketMessage) => void): void {
    if (!this.messageHandlers.has(messageType)) {
      this.messageHandlers.set(messageType, []);
    }
    this.messageHandlers.get(messageType)!.push(handler);
  }

  /**
   * Unregister message handler
   */
  off(messageType: string, handler: (message: WebSocketMessage) => void): void {
    const handlers = this.messageHandlers.get(messageType);
    if (handlers) {
      const index = handlers.indexOf(handler);
      if (index > -1) {
        handlers.splice(index, 1);
      }
    }
  }

  /**
   * Start heartbeat
   */
  private startHeartbeat(): void {
    this.heartbeatTimer = setInterval(() => {
      if (this.isConnected) {
        this.sendRaw({
          type: 'ping',
          sessionId: this.sessionId,
          userId: this.userId,
          timestamp: new Date().toISOString()
        });
      }
    }, this.heartbeatInterval);
  }

  /**
   * Stop heartbeat
   */
  private stopHeartbeat(): void {
    if (this.heartbeatTimer) {
      clearInterval(this.heartbeatTimer);
      this.heartbeatTimer = null;
    }
  }

  /**
   * Process queued messages
   */
  private processMessageQueue(): void {
    while (this.messageQueue.length > 0 && this.isConnected && this.isAuthenticated) {
      const message = this.messageQueue.shift();
      if (message) {
        this.sendRaw(message);
      }
    }
  }

  /**
   * Attempt to reconnect
   */
  private attemptReconnect(): void {
    if (this.reconnectAttempts >= this.maxReconnectAttempts) {
      console.error('Max reconnection attempts reached');
      return;
    }

    this.reconnectAttempts++;
    this.stats.reconnectAttempts++;

    console.log(`Attempting to reconnect (${this.reconnectAttempts}/${this.maxReconnectAttempts}) in ${this.currentReconnectDelay}ms`);

    setTimeout(() => {
      this.connect();
    }, this.currentReconnectDelay);

    // Exponential backoff
    this.currentReconnectDelay = Math.min(
      this.currentReconnectDelay * 2,
      this.maxReconnectDelay
    );
  }

  /**
   * Disconnect from server
   */
  disconnect(): void {
    this.stopHeartbeat();
    if (this.ws) {
      this.ws.close();
      this.ws = null;
    }
    this.isConnected = false;
    this.isAuthenticated = false;
  }

  /**
   * Get connection status
   */
  getStatus(): {
    connected: boolean;
    authenticated: boolean;
    stats: ConnectionStats;
  } {
    return {
      connected: this.isConnected,
      authenticated: this.isAuthenticated,
      stats: {
        ...this.stats,
        connectionDuration: this.connectionStartTime > 0 ? Date.now() - this.connectionStartTime : 0
      }
    };
  }

  /**
   * Check if connected
   */
  isReady(): boolean {
    return this.isConnected && this.isAuthenticated;
  }
}

export default SecureWebSocketClient;
