/**
 * Collaboration Context
 * Provides WebSocket connection and collaboration state to React components
 */

import React, { createContext, useContext, useEffect, useState, useCallback, useRef } from 'react';
import { WebSocketManager, CollaborationMessage, MessageType } from '@/lib/websocket';

export interface UserPresence {
  userId: string;
  userName: string;
  cursorPosition: number;
  selectionStart: number;
  selectionEnd: number;
  isOnline: boolean;
  lastActivity: number;
  color: string;
}

export interface CollaborationSession {
  sessionId: string;
  title: string;
  createdBy: string;
  participants: UserPresence[];
  documentVersion: number;
  lastModified: number;
  isActive: boolean;
}

export interface CollaborationContextType {
  // Connection state
  isConnected: boolean;
  sessionId: string | null;
  userId: string | null;
  
  // Session state
  session: CollaborationSession | null;
  participants: UserPresence[];
  
  // Operations
  joinSession: (sessionId: string, userId: string, userName: string) => Promise<void>;
  leaveSession: () => void;
  sendEdit: (operation: any) => void;
  updatePresence: (cursorPosition: number, selectionStart: number, selectionEnd: number) => void;
  addComment: (content: string, lineNumber: number) => void;
  
  // Message handlers
  onMessage: (type: MessageType, handler: (message: CollaborationMessage) => void) => void;
}

const CollaborationContext = createContext<CollaborationContextType | undefined>(undefined);

interface CollaborationProviderProps {
  children: React.ReactNode;
  wsUrl: string;
}

export const CollaborationProvider: React.FC<CollaborationProviderProps> = ({ children, wsUrl }) => {
  const [isConnected, setIsConnected] = useState(false);
  const [sessionId, setSessionId] = useState<string | null>(null);
  const [userId, setUserId] = useState<string | null>(null);
  const [session, setSession] = useState<CollaborationSession | null>(null);
  const [participants, setParticipants] = useState<UserPresence[]>([]);
  
  const wsManagerRef = useRef<WebSocketManager | null>(null);
  const presenceTimerRef = useRef<NodeJS.Timeout | null>(null);

  // Initialize WebSocket manager
  useEffect(() => {
    wsManagerRef.current = new WebSocketManager({
      url: wsUrl,
      reconnectAttempts: 5,
      reconnectDelay: 1000,
      maxReconnectDelay: 30000,
      heartbeatInterval: 30000,
      messageQueueSize: 1000,
    });

    return () => {
      if (wsManagerRef.current) {
        wsManagerRef.current.disconnect();
      }
    };
  }, [wsUrl]);

  // Handle connection state changes
  const handleConnectionChange = useCallback((connected: boolean) => {
    setIsConnected(connected);
  }, []);

  // Handle join message
  const handleJoinMessage = useCallback((message: CollaborationMessage) => {
    if (message.data.sessionId) {
      setSessionId(message.data.sessionId);
      setSession({
        sessionId: message.data.sessionId,
        title: message.data.title || 'Untitled Session',
        createdBy: message.data.createdBy,
        participants: message.data.participants || [],
        documentVersion: message.data.documentVersion || 0,
        lastModified: message.timestamp,
        isActive: true,
      });
      setParticipants(message.data.participants || []);
    }
  }, []);

  // Handle presence message
  const handlePresenceMessage = useCallback((message: CollaborationMessage) => {
    setParticipants((prev) => {
      const updated = [...prev];
      const index = updated.findIndex((p) => p.userId === message.userId);
      if (index >= 0) {
        updated[index] = {
          ...updated[index],
          ...message.data,
          lastActivity: message.timestamp,
        };
      } else {
        updated.push({
          userId: message.userId,
          userName: message.data.userName || 'Unknown',
          cursorPosition: message.data.cursorPosition || 0,
          selectionStart: message.data.selectionStart || 0,
          selectionEnd: message.data.selectionEnd || 0,
          isOnline: true,
          lastActivity: message.timestamp,
          color: message.data.color || '#000000',
        });
      }
      return updated;
    });
  }, []);

  // Handle leave message
  const handleLeaveMessage = useCallback((message: CollaborationMessage) => {
    setParticipants((prev) =>
      prev.filter((p) => p.userId !== message.userId)
    );
  }, []);

  // Handle sync message
  const handleSyncMessage = useCallback((message: CollaborationMessage) => {
    if (message.data.sessionId) {
      setSession((prev) =>
        prev
          ? {
              ...prev,
              documentVersion: message.revision || prev.documentVersion,
              lastModified: message.timestamp,
            }
          : null
      );
    }
  }, []);

  // Join session
  const joinSession = useCallback(
    async (sid: string, uid: string, userName: string) => {
      if (!wsManagerRef.current) {
        throw new Error('WebSocket manager not initialized');
      }

      setUserId(uid);
      setSessionId(sid);

      // Connect to WebSocket
      await wsManagerRef.current.connect(uid, sid);

      // Register message handlers
      wsManagerRef.current.onConnectionChange(handleConnectionChange);
      wsManagerRef.current.onMessage('join', handleJoinMessage);
      wsManagerRef.current.onMessage('presence', handlePresenceMessage);
      wsManagerRef.current.onMessage('leave', handleLeaveMessage);
      wsManagerRef.current.onMessage('sync', handleSyncMessage);

      // Send join message
      wsManagerRef.current.sendMessage({
        type: 'join',
        sessionId: sid,
        userId: uid,
        timestamp: Date.now(),
        data: {
          userName,
          userColor: generateUserColor(),
        },
      });
    },
    [handleConnectionChange, handleJoinMessage, handlePresenceMessage, handleLeaveMessage, handleSyncMessage]
  );

  // Leave session
  const leaveSession = useCallback(() => {
    if (!wsManagerRef.current || !sessionId || !userId) {
      return;
    }

    wsManagerRef.current.sendMessage({
      type: 'leave',
      sessionId,
      userId,
      timestamp: Date.now(),
      data: {},
    });

    wsManagerRef.current.disconnect();
    setSessionId(null);
    setUserId(null);
    setSession(null);
    setParticipants([]);
    setIsConnected(false);

    if (presenceTimerRef.current) {
      clearInterval(presenceTimerRef.current);
    }
  }, [sessionId, userId]);

  // Send edit operation
  const sendEdit = useCallback(
    (operation: any) => {
      if (!wsManagerRef.current || !sessionId || !userId) {
        return;
      }

      wsManagerRef.current.sendMessage({
        type: 'edit',
        sessionId,
        userId,
        timestamp: Date.now(),
        data: { operation },
      });
    },
    [sessionId, userId]
  );

  // Update presence
  const updatePresence = useCallback(
    (cursorPosition: number, selectionStart: number, selectionEnd: number) => {
      if (!wsManagerRef.current || !sessionId || !userId) {
        return;
      }

      wsManagerRef.current.sendMessage({
        type: 'presence',
        sessionId,
        userId,
        timestamp: Date.now(),
        data: {
          cursorPosition,
          selectionStart,
          selectionEnd,
          isOnline: true,
        },
      });
    },
    [sessionId, userId]
  );

  // Add comment
  const addComment = useCallback(
    (content: string, lineNumber: number) => {
      if (!wsManagerRef.current || !sessionId || !userId) {
        return;
      }

      wsManagerRef.current.sendMessage({
        type: 'comment',
        sessionId,
        userId,
        timestamp: Date.now(),
        data: {
          content,
          lineNumber,
          commentId: `comment-${Date.now()}`,
        },
      });
    },
    [sessionId, userId]
  );

  // Register message handler
  const onMessage = useCallback(
    (type: MessageType, handler: (message: CollaborationMessage) => void) => {
      if (wsManagerRef.current) {
        wsManagerRef.current.onMessage(type, handler);
      }
    },
    []
  );

  const value: CollaborationContextType = {
    isConnected,
    sessionId,
    userId,
    session,
    participants,
    joinSession,
    leaveSession,
    sendEdit,
    updatePresence,
    addComment,
    onMessage,
  };

  return (
    <CollaborationContext.Provider value={value}>
      {children}
    </CollaborationContext.Provider>
  );
};

/**
 * Hook to use collaboration context
 */
export const useCollaboration = (): CollaborationContextType => {
  const context = useContext(CollaborationContext);
  if (!context) {
    throw new Error('useCollaboration must be used within CollaborationProvider');
  }
  return context;
};

/**
 * Generate random user color
 */
function generateUserColor(): string {
  const colors = [
    '#FF6B6B', // Red
    '#4ECDC4', // Teal
    '#45B7D1', // Blue
    '#FFA07A', // Salmon
    '#98D8C8', // Mint
    '#F7DC6F', // Yellow
    '#BB8FCE', // Purple
    '#85C1E2', // Light Blue
  ];
  return colors[Math.floor(Math.random() * colors.length)];
}
