/**
 * CommentThread Component
 * Displays threaded comments for collaborative feedback
 */

import React, { useState, useCallback } from 'react';
import { useCollaboration } from '@/contexts/CollaborationContext';
import { Button } from '@/components/ui/button';
import { Card } from '@/components/ui/card';
import { MessageCircle, X, Check } from 'lucide-react';

export interface Comment {
  id: string;
  author: string;
  authorId: string;
  content: string;
  timestamp: number;
  lineNumber: number;
  resolved: boolean;
  replies: Comment[];
  color?: string;
}

interface CommentThreadProps {
  lineNumber: number;
  comments: Comment[];
  onAddComment?: (content: string, lineNumber: number) => void;
  onResolveComment?: (commentId: string) => void;
  onDeleteComment?: (commentId: string) => void;
}

export const CommentThread: React.FC<CommentThreadProps> = ({
  lineNumber,
  comments,
  onAddComment,
  onResolveComment,
  onDeleteComment,
}) => {
  const [isExpanded, setIsExpanded] = useState(false);
  const [newComment, setNewComment] = useState('');
  const { userId } = useCollaboration();

  const handleAddComment = useCallback(() => {
    if (newComment.trim()) {
      onAddComment?.(newComment, lineNumber);
      setNewComment('');
    }
  }, [newComment, lineNumber, onAddComment]);

  const unresolvedCount = comments.filter((c) => !c.resolved).length;

  return (
    <div className="relative">
      {/* Comment indicator */}
      <button
        onClick={() => setIsExpanded(!isExpanded)}
        className="inline-flex items-center gap-1 px-2 py-1 text-xs font-medium rounded-full bg-amber-100 dark:bg-amber-900/30 text-amber-700 dark:text-amber-300 hover:bg-amber-200 dark:hover:bg-amber-900/50 transition-colors"
      >
        <MessageCircle className="w-3 h-3" />
        {unresolvedCount}
      </button>

      {/* Comment thread panel */}
      {isExpanded && (
        <div className="absolute top-full left-0 mt-2 w-80 bg-white dark:bg-slate-900 border border-slate-200 dark:border-slate-700 rounded-lg shadow-lg z-50">
          <div className="p-4">
            {/* Header */}
            <div className="flex items-center justify-between mb-4">
              <h3 className="font-semibold text-slate-900 dark:text-white">Line {lineNumber} Comments</h3>
              <button
                onClick={() => setIsExpanded(false)}
                className="text-slate-400 hover:text-slate-600 dark:hover:text-slate-300 transition-colors"
              >
                <X className="w-4 h-4" />
              </button>
            </div>

            {/* Comments list */}
            <div className="space-y-3 mb-4 max-h-64 overflow-y-auto">
              {comments.map((comment) => (
                <CommentItem
                  key={comment.id}
                  comment={comment}
                  isAuthor={comment.authorId === userId}
                  onResolve={() => onResolveComment?.(comment.id)}
                  onDelete={() => onDeleteComment?.(comment.id)}
                />
              ))}
              {comments.length === 0 && (
                <p className="text-sm text-slate-500 dark:text-slate-400 italic">No comments yet</p>
              )}
            </div>

            {/* Add comment form */}
            <div className="border-t border-slate-200 dark:border-slate-700 pt-4">
              <textarea
                value={newComment}
                onChange={(e) => setNewComment(e.target.value)}
                placeholder="Add a comment..."
                className="w-full px-3 py-2 text-sm border border-slate-300 dark:border-slate-600 rounded-md bg-white dark:bg-slate-800 text-slate-900 dark:text-white placeholder-slate-400 dark:placeholder-slate-500 focus:outline-none focus:ring-2 focus:ring-blue-500 resize-none"
                rows={3}
              />
              <div className="flex gap-2 mt-2">
                <Button
                  onClick={handleAddComment}
                  disabled={!newComment.trim()}
                  size="sm"
                  className="flex-1"
                >
                  Comment
                </Button>
                <Button
                  onClick={() => setNewComment('')}
                  variant="outline"
                  size="sm"
                  className="flex-1"
                >
                  Cancel
                </Button>
              </div>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

interface CommentItemProps {
  comment: Comment;
  isAuthor: boolean;
  onResolve?: () => void;
  onDelete?: () => void;
}

const CommentItem: React.FC<CommentItemProps> = ({
  comment,
  isAuthor,
  onResolve,
  onDelete,
}) => {
  const formatTime = (timestamp: number) => {
    const date = new Date(timestamp);
    const now = new Date();
    const diffMs = now.getTime() - date.getTime();
    const diffMins = Math.floor(diffMs / 60000);

    if (diffMins < 1) return 'just now';
    if (diffMins < 60) return `${diffMins}m ago`;
    if (diffMins < 1440) return `${Math.floor(diffMins / 60)}h ago`;
    return date.toLocaleDateString();
  };

  return (
    <Card className={`p-3 ${comment.resolved ? 'opacity-60' : ''}`}>
      {/* Comment header */}
      <div className="flex items-start justify-between mb-2">
        <div className="flex items-center gap-2">
          <div
            className="w-6 h-6 rounded-full flex items-center justify-center text-white text-xs font-bold"
            style={{ backgroundColor: comment.color || '#999' }}
          >
            {comment.author.charAt(0).toUpperCase()}
          </div>
          <div>
            <p className="text-sm font-medium text-slate-900 dark:text-white">{comment.author}</p>
            <p className="text-xs text-slate-500 dark:text-slate-400">{formatTime(comment.timestamp)}</p>
          </div>
        </div>
        {isAuthor && (
          <button
            onClick={onDelete}
            className="text-slate-400 hover:text-red-600 dark:hover:text-red-400 transition-colors"
            title="Delete comment"
          >
            <X className="w-4 h-4" />
          </button>
        )}
      </div>

      {/* Comment content */}
      <p className="text-sm text-slate-700 dark:text-slate-300 mb-3">{comment.content}</p>

      {/* Comment actions */}
      <div className="flex items-center gap-2">
        {!comment.resolved && (
          <button
            onClick={onResolve}
            className="inline-flex items-center gap-1 text-xs font-medium text-green-600 dark:text-green-400 hover:text-green-700 dark:hover:text-green-300 transition-colors"
          >
            <Check className="w-3 h-3" />
            Resolve
          </button>
        )}
        {comment.resolved && (
          <span className="inline-flex items-center gap-1 text-xs font-medium text-green-600 dark:text-green-400">
            <Check className="w-3 h-3" />
            Resolved
          </span>
        )}
      </div>

      {/* Replies */}
      {comment.replies && comment.replies.length > 0 && (
        <div className="mt-3 pl-4 border-l-2 border-slate-200 dark:border-slate-700 space-y-2">
          {comment.replies.map((reply) => (
            <CommentItem
              key={reply.id}
              comment={reply}
              isAuthor={reply.authorId === comment.authorId}
              onResolve={onResolve}
              onDelete={onDelete}
            />
          ))}
        </div>
      )}
    </Card>
  );
};
