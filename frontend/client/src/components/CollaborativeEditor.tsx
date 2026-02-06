/**
 * CollaborativeEditor Component
 * Integrates WebSocket, OT, presence, and comments for real-time collaborative editing
 */

import React, { useRef, useEffect, useState, useCallback } from 'react';
import { useCollaboration } from '@/contexts/CollaborationContext';
import { useOTEngine } from '@/hooks/useOTEngine';
import { useCursorTracking } from '@/hooks/usePresenceTracking';
import { CollaborationToolbar, RemotePresence, PresenceIndicator } from './RemotePresence';
import { CommentThread, Comment } from './CommentThread';
import { createInsertOperation, createDeleteOperation } from '@/lib/ot-engine';

interface CollaborativeEditorProps {
  initialContent?: string;
  onContentChange?: (content: string) => void;
  readOnly?: boolean;
  placeholder?: string;
}

export const CollaborativeEditor: React.FC<CollaborativeEditorProps> = ({
  initialContent = '',
  onContentChange,
  readOnly = false,
  placeholder = 'Start typing...',
}) => {
  const editorRef = useRef<HTMLTextAreaElement>(null);
  const [content, setContent] = useState(initialContent);
  const [comments, setComments] = useState<Map<number, Comment[]>>(new Map());
  const [selectedLineNumber, setSelectedLineNumber] = useState<number | null>(null);

  const { isConnected, sendEdit, addComment: sendComment, onMessage } = useCollaboration();
  const { applyLocalOperation, applyToText, canUndo, canRedo, undo, redo } = useOTEngine();

  // Track cursor position
  useCursorTracking(editorRef as React.RefObject<HTMLTextAreaElement>);

  // Handle local edits
  const handleChange = useCallback(
    (e: React.ChangeEvent<HTMLTextAreaElement>) => {
      const newContent = e.target.value;
      const oldContent = content;

      // Determine what changed
      if (newContent.length > oldContent.length) {
        // Insert
        const insertPos = e.target.selectionStart - (newContent.length - oldContent.length);
        const insertedText = newContent.substring(insertPos, e.target.selectionStart);

        const operation = createInsertOperation(insertPos, insertedText);
        applyLocalOperation(operation);
      } else if (newContent.length < oldContent.length) {
        // Delete
        const deletePos = e.target.selectionStart;
        const deletedLength = oldContent.length - newContent.length;

        const operation = createDeleteOperation(deletePos, deletedLength);
        applyLocalOperation(operation);
      }

      setContent(newContent);
      onContentChange?.(newContent);
    },
    [content, applyLocalOperation, onContentChange]
  );

  // Handle remote edits
  useEffect(() => {
    const handleRemoteEdit = (message: any) => {
      if (message.data.operation) {
        const transformedContent = applyToText(content, message.data.operation);
        setContent(transformedContent);
        onContentChange?.(transformedContent);
      }
    };

    onMessage('edit', handleRemoteEdit);
  }, [content, onMessage, applyToText, onContentChange]);

  // Handle remote comments
  useEffect(() => {
    const handleRemoteComment = (message: any) => {
      if (message.data.commentId) {
        const lineNumber = message.data.lineNumber;
        const newComment: Comment = {
          id: message.data.commentId,
          author: message.data.author || 'Unknown',
          authorId: message.userId,
          content: message.data.content,
          timestamp: message.timestamp,
          lineNumber,
          resolved: false,
          replies: [],
          color: message.data.color,
        };

        setComments((prev) => {
          const updated = new Map(prev);
          const lineComments = updated.get(lineNumber) || [];
          updated.set(lineNumber, [...lineComments, newComment]);
          return updated;
        });
      }
    };

    onMessage('comment', handleRemoteComment);
  }, [onMessage]);

  // Handle undo
  const handleUndo = useCallback(() => {
    const undoOp = undo();
    if (undoOp) {
      const newContent = applyToText(content, undoOp);
      setContent(newContent);
      onContentChange?.(newContent);
    }
  }, [content, undo, applyToText, onContentChange]);

  // Handle redo
  const handleRedo = useCallback(() => {
    const redoOp = redo();
    if (redoOp) {
      const newContent = applyToText(content, redoOp);
      setContent(newContent);
      onContentChange?.(newContent);
    }
  }, [content, redo, applyToText, onContentChange]);

  // Handle add comment
  const handleAddComment = useCallback(
    (commentText: string, lineNumber: number) => {
      sendComment(commentText, lineNumber);

      const newComment: Comment = {
        id: `comment-${Date.now()}`,
        author: 'You',
        authorId: 'current-user',
        content: commentText,
        timestamp: Date.now(),
        lineNumber,
        resolved: false,
        replies: [],
      };

      setComments((prev) => {
        const updated = new Map(prev);
        const lineComments = updated.get(lineNumber) || [];
        updated.set(lineNumber, [...lineComments, newComment]);
        return updated;
      });
    },
    [sendComment]
  );

  // Handle resolve comment
  const handleResolveComment = useCallback((commentId: string) => {
    setComments((prev) => {
      const updated = new Map(prev);
      updated.forEach((lineComments) => {
        lineComments.forEach((comment) => {
          if (comment.id === commentId) {
            comment.resolved = true;
          }
        });
      });
      return updated;
    });
  }, []);

  // Handle delete comment
  const handleDeleteComment = useCallback((commentId: string) => {
    setComments((prev) => {
      const updated = new Map(prev);
      updated.forEach((lineComments, lineNumber) => {
        const filtered = lineComments.filter((c) => c.id !== commentId);
        if (filtered.length === 0) {
          updated.delete(lineNumber);
        } else {
          updated.set(lineNumber, filtered);
        }
      });
      return updated;
    });
  }, []);

  // Get line number from cursor position
  const getLineNumber = useCallback(() => {
    if (!editorRef.current) return 0;
    const text = editorRef.current.value;
    const cursorPos = editorRef.current.selectionStart;
    return text.substring(0, cursorPos).split('\n').length;
  }, []);

  // Handle line click to add comment
  const handleLineClick = useCallback(() => {
    const lineNumber = getLineNumber();
    setSelectedLineNumber(lineNumber);
  }, [getLineNumber]);

  return (
    <div className="flex flex-col h-full bg-white dark:bg-slate-950 rounded-lg overflow-hidden border border-slate-200 dark:border-slate-800">
      {/* Toolbar */}
      <CollaborationToolbar />

      {/* Editor container */}
      <div className="flex-1 flex overflow-hidden">
        {/* Line numbers */}
        <div className="w-12 bg-slate-50 dark:bg-slate-900 border-r border-slate-200 dark:border-slate-700 flex flex-col items-center py-4 text-xs text-slate-500 dark:text-slate-400 overflow-hidden">
          {content.split('\n').map((_, i) => (
            <div
              key={i}
              className="h-6 flex items-center justify-center cursor-pointer hover:bg-slate-200 dark:hover:bg-slate-800 w-full transition-colors"
              onClick={() => setSelectedLineNumber(i + 1)}
            >
              {i + 1}
            </div>
          ))}
        </div>

        {/* Editor */}
        <div className="flex-1 relative overflow-hidden">
          {/* Remote cursors */}
          <div className="absolute inset-0 pointer-events-none">
            <RemotePresence editorHeight={400} editorWidth={800} lineHeight={24} charWidth={8} />
          </div>

          {/* Textarea */}
          <textarea
            ref={editorRef}
            value={content}
            onChange={handleChange}
            onClick={handleLineClick}
            readOnly={readOnly}
            placeholder={placeholder}
            className="absolute inset-0 w-full h-full p-4 font-mono text-sm bg-white dark:bg-slate-950 text-slate-900 dark:text-white placeholder-slate-400 dark:placeholder-slate-600 focus:outline-none resize-none"
            style={{
              lineHeight: '24px',
              letterSpacing: '0.5px',
            }}
          />
        </div>

        {/* Comments sidebar */}
        <div className="w-80 bg-slate-50 dark:bg-slate-900 border-l border-slate-200 dark:border-slate-700 overflow-y-auto p-4">
          {selectedLineNumber && comments.has(selectedLineNumber) && (
            <CommentThread
              lineNumber={selectedLineNumber}
              comments={comments.get(selectedLineNumber) || []}
              onAddComment={handleAddComment}
              onResolveComment={handleResolveComment}
              onDeleteComment={handleDeleteComment}
            />
          )}
          {selectedLineNumber && !comments.has(selectedLineNumber) && (
            <div className="text-sm text-slate-500 dark:text-slate-400">
              <p className="mb-3">No comments on line {selectedLineNumber}</p>
              <button
                onClick={() => {
                  const comment = prompt('Add a comment:');
                  if (comment) {
                    handleAddComment(comment, selectedLineNumber);
                  }
                }}
                className="px-3 py-2 bg-blue-500 hover:bg-blue-600 text-white rounded-md text-xs font-medium transition-colors"
              >
                Add Comment
              </button>
            </div>
          )}
        </div>
      </div>

      {/* Status bar */}
      <div className="flex items-center justify-between gap-4 p-3 bg-slate-50 dark:bg-slate-900 border-t border-slate-200 dark:border-slate-700 text-xs text-slate-600 dark:text-slate-400">
        <div className="flex items-center gap-4">
          <span>{content.length} characters</span>
          <span>{content.split('\n').length} lines</span>
          <span className={isConnected ? 'text-green-600 dark:text-green-400' : 'text-red-600 dark:text-red-400'}>
            {isConnected ? 'Connected' : 'Disconnected'}
          </span>
        </div>
        <div className="flex items-center gap-2">
          <button
            onClick={handleUndo}
            disabled={!canUndo}
            className="px-2 py-1 hover:bg-slate-200 dark:hover:bg-slate-800 disabled:opacity-50 disabled:cursor-not-allowed rounded transition-colors"
            title="Undo"
          >
            ↶
          </button>
          <button
            onClick={handleRedo}
            disabled={!canRedo}
            className="px-2 py-1 hover:bg-slate-200 dark:hover:bg-slate-800 disabled:opacity-50 disabled:cursor-not-allowed rounded transition-colors"
            title="Redo"
          >
            ↷
          </button>
        </div>
      </div>
    </div>
  );
};
