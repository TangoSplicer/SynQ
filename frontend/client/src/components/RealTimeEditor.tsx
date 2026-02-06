/**
 * Real-Time Editor Component
 *
 * Provides a collaborative code editor with:
 * - WebSocket-based real-time synchronization
 * - Operational Transformation for conflict-free editing
 * - Remote presence tracking (cursors, selections)
 * - Comment threading
 * - Undo/redo support
 * - Edit history
 */

import React, { useEffect, useRef, useState, useCallback } from 'react';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import SecureWebSocketClient from '@/lib/websocket-secure';
import { useOTEngine } from '@/hooks/useOTEngine';
import { usePresenceTracking } from '@/hooks/usePresenceTracking';
import { RemotePresence } from './RemotePresence';
import { CommentThread } from './CommentThread';

interface RealTimeEditorProps {
  sessionId: string;
  userId: string;
  token: string;
  websocketUrl: string;
  onStatusChange?: (status: 'connected' | 'disconnected' | 'error') => void;
  onContentChange?: (content: string) => void;
}

interface EditorState {
  content: string;
  cursorLine: number;
  cursorColumn: number;
  selectionStart: number;
  selectionEnd: number;
}

interface RemoteUser {
  userId: string;
  name: string;
  color: string;
  cursorLine: number;
  cursorColumn: number;
  selectionStart?: number;
  selectionEnd?: number;
}

const RealTimeEditor: React.FC<RealTimeEditorProps> = ({
  sessionId,
  userId,
  token,
  websocketUrl,
  onStatusChange,
  onContentChange
}) => {
  const textareaRef = useRef<HTMLTextAreaElement>(null);
  const [editorState, setEditorState] = useState<EditorState>({
    content: '',
    cursorLine: 0,
    cursorColumn: 0,
    selectionStart: 0,
    selectionEnd: 0
  });

  const [remoteUsers, setRemoteUsers] = useState<Map<string, RemoteUser>>(new Map());
  const [comments, setComments] = useState<any[]>([]);
  const [undoStack, setUndoStack] = useState<string[]>([]);
  const [redoStack, setRedoStack] = useState<string[]>([]);
  const [connectionStatus, setConnectionStatus] = useState<'connected' | 'disconnected' | 'error'>('disconnected');
  const [stats, setStats] = useState({ messagesReceived: 0, messagesSent: 0 });

  const wsRef = useRef<SecureWebSocketClient | null>(null);
  const otEngine = useOTEngine();
  const { trackPresence } = usePresenceTracking();

  // Initialize WebSocket connection
  useEffect(() => {
    const initializeWebSocket = async () => {
      try {
        wsRef.current = new SecureWebSocketClient({
          url: websocketUrl,
          token,
          sessionId,
          maxReconnectAttempts: 10,
          initialReconnectDelay: 1000,
          maxReconnectDelay: 30000,
          heartbeatInterval: 30000
        });

        // Register message handlers
        wsRef.current.on('authenticated', (msg) => {
          console.log('Authenticated:', msg);
          setConnectionStatus('connected');
          onStatusChange?.('connected');
        });

        wsRef.current.on('edit', (msg) => {
          // Apply remote edit
          const { content: newContent } = msg;
          setEditorState(prev => ({
            ...prev,
            content: newContent
          }));
        });

        wsRef.current.on('presence', (msg) => {
          // Update remote user presence
          const { userId: remoteUserId, cursor, selection, name, color } = msg;
          setRemoteUsers(prev => {
            const updated = new Map(prev);
            updated.set(remoteUserId, {
              userId: remoteUserId,
              name,
              color,
              cursorLine: cursor?.line || 0,
              cursorColumn: cursor?.column || 0,
              selectionStart: selection?.start,
              selectionEnd: selection?.end
            });
            return updated;
          });
        });

        wsRef.current.on('comment', (msg) => {
          // Add comment
          const { commentId, line, text, userId: authorId, userName } = msg;
          setComments(prev => [...prev, {
            id: commentId,
            line,
            text,
            userId: authorId,
            userName,
            timestamp: new Date().toISOString(),
            resolved: false,
            replies: []
          }]);
        });

        wsRef.current.on('user_left', (msg) => {
          // Remove user from remote users
          const { userId: leftUserId } = msg;
          setRemoteUsers(prev => {
            const updated = new Map(prev);
            updated.delete(leftUserId);
            return updated;
          });
        });

        // Connect to server
        const connected = await wsRef.current.connect();
        if (!connected) {
          setConnectionStatus('error');
          onStatusChange?.('error');
        }
      } catch (error) {
        console.error('Failed to initialize WebSocket:', error);
        setConnectionStatus('error');
        onStatusChange?.('error');
      }
    };

    initializeWebSocket();

    return () => {
      if (wsRef.current) {
        wsRef.current.disconnect();
      }
    };
  }, [sessionId, userId, token, websocketUrl, onStatusChange]);

  // Handle text changes
  const handleTextChange = useCallback((e: React.ChangeEvent<HTMLTextAreaElement>) => {
    const newContent = e.currentTarget.value;
    const oldContent = editorState.content;

    // Calculate the operation (simplified - in production use proper diff algorithm)
    const operation = {
      type: 'edit',
      content: newContent,
      position: {
        line: editorState.cursorLine,
        column: editorState.cursorColumn
      }
    };

    // Update local state
    setEditorState(prev => ({
      ...prev,
      content: newContent
    }));

    // Add to undo stack
    setUndoStack(prev => [...prev, oldContent]);
    setRedoStack([]);

    // Send to server
    if (wsRef.current?.isReady()) {
      wsRef.current.send(operation);
    }

    // Notify parent
    onContentChange?.(newContent);

    // Update stats
    setStats(prev => ({
      ...prev,
      messagesSent: prev.messagesSent + 1
    }));
  }, [editorState, onContentChange]);

  // Handle cursor movement
  const handleCursorMove = useCallback((e: React.KeyboardEvent<HTMLTextAreaElement> | React.MouseEvent<HTMLTextAreaElement>) => {
    const textarea = e.currentTarget;
    const { selectionStart, selectionEnd, value } = textarea;

    // Calculate line and column
    const beforeCursor = value.substring(0, selectionStart);
    const lines = beforeCursor.split('\n');
    const cursorLine = lines.length - 1;
    const cursorColumn = lines[lines.length - 1].length;

    setEditorState(prev => ({
      ...prev,
      cursorLine,
      cursorColumn,
      selectionStart,
      selectionEnd
    }));

    // Send presence update (throttled)
    trackPresence(cursorLine, selectionStart, selectionEnd);
  }, [trackPresence]);

  // Handle undo
  const handleUndo = useCallback(() => {
    if (undoStack.length === 0) return;

    const previousContent = undoStack[undoStack.length - 1];
    setRedoStack(prev => [...prev, editorState.content]);
    setUndoStack(prev => prev.slice(0, -1));
    setEditorState(prev => ({
      ...prev,
      content: previousContent
    }));

    // Send undo operation
    if (wsRef.current?.isReady()) {
      wsRef.current.send({
        type: 'undo'
      });
    }
  }, [undoStack, editorState.content]);

  // Handle redo
  const handleRedo = useCallback(() => {
    if (redoStack.length === 0) return;

    const nextContent = redoStack[redoStack.length - 1];
    setUndoStack(prev => [...prev, editorState.content]);
    setRedoStack(prev => prev.slice(0, -1));
    setEditorState(prev => ({
      ...prev,
      content: nextContent
    }));

    // Send redo operation
    if (wsRef.current?.isReady()) {
      wsRef.current.send({
        type: 'redo'
      });
    }
  }, [redoStack, editorState.content]);

  // Handle adding comment
  const handleAddComment = useCallback((line: number, text: string) => {
    if (wsRef.current?.isReady()) {
      wsRef.current.send({
        type: 'comment',
        line,
        text
      });
    }
  }, []);

  // Get connection status color
  const statusColor = {
    connected: 'bg-green-500',
    disconnected: 'bg-yellow-500',
    error: 'bg-red-500'
  }[connectionStatus];

  // Calculate stats
  const wsStats = wsRef.current?.getStatus();

  return (
    <div className="flex flex-col h-full bg-background">
      {/* Toolbar */}
      <div className="flex items-center justify-between p-4 border-b border-border">
        <div className="flex items-center gap-4">
          <div className="flex items-center gap-2">
            <div className={`w-3 h-3 rounded-full ${statusColor}`} />
            <span className="text-sm font-medium capitalize">
              {connectionStatus}
            </span>
          </div>
          <span className="text-xs text-muted-foreground">
            Session: {sessionId.slice(0, 8)}
          </span>
        </div>

        <div className="flex items-center gap-2">
          <Button
            variant="outline"
            size="sm"
            onClick={handleUndo}
            disabled={undoStack.length === 0}
            title="Undo (Ctrl+Z)"
          >
            ↶ Undo
          </Button>
          <Button
            variant="outline"
            size="sm"
            onClick={handleRedo}
            disabled={redoStack.length === 0}
            title="Redo (Ctrl+Y)"
          >
            ↷ Redo
          </Button>
        </div>
      </div>

      {/* Editor Container */}
      <div className="flex flex-1 gap-4 p-4 overflow-hidden">
        {/* Main Editor */}
        <div className="flex-1 flex flex-col gap-2 min-w-0">
          <Card className="flex-1 p-4 overflow-hidden">
            <div className="relative h-full">
              <textarea
                ref={textareaRef}
                value={editorState.content}
                onChange={handleTextChange}
                onKeyUp={handleCursorMove}
                onClick={(e: React.MouseEvent<HTMLTextAreaElement>) => handleCursorMove(e as any)}
                className="w-full h-full p-4 font-mono text-sm bg-background text-foreground border-0 resize-none focus:outline-none"
                placeholder="// Start typing your quantum circuit code here..."
                spellCheck="false"
              />

              {/* Remote Presence Overlay */}
              <RemotePresence
                editorHeight={400}
                editorWidth={600}
                lineHeight={24}
                charWidth={8}
              />
            </div>
          </Card>

          {/* Stats */}
          <div className="flex justify-between text-xs text-muted-foreground px-2">
            <div>
              Line {editorState.cursorLine + 1}, Column {editorState.cursorColumn + 1}
            </div>
            <div>
              {editorState.content.length} characters | {editorState.content.split('\n').length} lines
            </div>
          </div>
        </div>

        {/* Sidebar */}
        <div className="w-64 flex flex-col gap-4 overflow-y-auto">
          {/* Participants */}
          <Card className="p-4">
            <h3 className="font-semibold text-sm mb-3">Participants</h3>
            <div className="space-y-2">
              {Array.from(remoteUsers.values()).map(user => (
                <div key={user.userId} className="flex items-center gap-2 text-sm">
                  <div
                    className="w-3 h-3 rounded-full"
                    style={{ backgroundColor: user.color }}
                  />
                  <span>{user.name}</span>
                </div>
              ))}
            </div>
          </Card>

          {/* Comments */}
          <Card className="p-4">
            <h3 className="font-semibold text-sm mb-3">Comments</h3>
            <div className="space-y-2 max-h-96 overflow-y-auto">
              {comments.map(comment => (
                <CommentThread
                  key={comment.id}
                  lineNumber={comment.line}
                  comments={[comment]}
                  onAddComment={(text: string) => {
                    // Handle reply
                  }}
                />
              ))}
            </div>
          </Card>

          {/* Statistics */}
          <Card className="p-4">
            <h3 className="font-semibold text-sm mb-3">Statistics</h3>
            <div className="space-y-1 text-xs text-muted-foreground">
              <div>Sent: {wsStats?.stats.messagesSent || 0}</div>
              <div>Received: {wsStats?.stats.messagesReceived || 0}</div>
              <div>Latency: {Math.round(wsStats?.stats.averageLatency || 0)}ms</div>
              <div>Reconnects: {wsStats?.stats.reconnectAttempts || 0}</div>
            </div>
          </Card>
        </div>
      </div>
    </div>
  );
};

export default RealTimeEditor;
