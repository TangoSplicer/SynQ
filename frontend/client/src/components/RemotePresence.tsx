/**
 * RemotePresence Component
 * Displays remote user cursors, selections, and presence indicators
 */

import React, { useMemo } from 'react';
import { useCollaboration, UserPresence } from '@/contexts/CollaborationContext';

interface RemotePresenceProps {
  editorHeight: number;
  editorWidth: number;
  lineHeight: number;
  charWidth: number;
}

export const RemotePresence: React.FC<RemotePresenceProps> = ({
  editorHeight,
  editorWidth,
  lineHeight,
  charWidth,
}) => {
  const { participants, userId } = useCollaboration();

  // Filter out current user
  const remoteUsers = useMemo(
    () => participants.filter((p) => p.userId !== userId && p.isOnline),
    [participants, userId]
  );

  return (
    <div className="relative pointer-events-none">
      {remoteUsers.map((user) => (
        <RemoteUserCursor
          key={user.userId}
          user={user}
          lineHeight={lineHeight}
          charWidth={charWidth}
        />
      ))}
    </div>
  );
};

interface RemoteUserCursorProps {
  user: UserPresence;
  lineHeight: number;
  charWidth: number;
}

const RemoteUserCursor: React.FC<RemoteUserCursorProps> = ({
  user,
  lineHeight,
  charWidth,
}) => {
  // Calculate cursor position
  const top = Math.floor(user.cursorPosition / 80) * lineHeight;
  const left = (user.cursorPosition % 80) * charWidth;

  // Calculate selection if exists
  const hasSelection = user.selectionStart !== user.selectionEnd;
  const selectionTop = Math.floor(user.selectionStart / 80) * lineHeight;
  const selectionLeft = (user.selectionStart % 80) * charWidth;
  const selectionHeight =
    (Math.floor(user.selectionEnd / 80) - Math.floor(user.selectionStart / 80) + 1) * lineHeight;

  return (
    <>
      {/* Selection highlight */}
      {hasSelection && (
        <div
          className="absolute opacity-20 pointer-events-none"
          style={{
            top: `${selectionTop}px`,
            left: `${selectionLeft}px`,
            height: `${selectionHeight}px`,
            width: '100%',
            backgroundColor: user.color,
            zIndex: 1,
          }}
        />
      )}

      {/* Cursor */}
      <div
        className="absolute pointer-events-none transition-all duration-75"
        style={{
          top: `${top}px`,
          left: `${left}px`,
          zIndex: 10,
        }}
      >
        {/* Cursor line */}
        <div
          className="absolute w-0.5 animate-pulse"
          style={{
            height: `${lineHeight}px`,
            backgroundColor: user.color,
            boxShadow: `0 0 4px ${user.color}`,
          }}
        />

        {/* User label */}
        <div
          className="absolute top-0 left-1 text-xs font-semibold text-white px-2 py-1 rounded whitespace-nowrap"
          style={{
            backgroundColor: user.color,
            transform: 'translateY(-100%)',
            marginTop: '-4px',
          }}
        >
          {user.userName}
        </div>
      </div>
    </>
  );
};

/**
 * Presence Indicator Component
 * Shows list of connected users
 */
interface PresenceIndicatorProps {
  maxDisplay?: number;
}

export const PresenceIndicator: React.FC<PresenceIndicatorProps> = ({ maxDisplay = 5 }) => {
  const { participants } = useCollaboration();

  const onlineUsers = useMemo(
    () => participants.filter((p) => p.isOnline),
    [participants]
  );

  const displayUsers = onlineUsers.slice(0, maxDisplay);
  const hiddenCount = Math.max(0, onlineUsers.length - maxDisplay);

  return (
    <div className="flex items-center gap-2">
      <span className="text-sm font-medium text-slate-600 dark:text-slate-400">
        {onlineUsers.length} online
      </span>
      <div className="flex -space-x-2">
        {displayUsers.map((user) => (
          <div
            key={user.userId}
            className="w-8 h-8 rounded-full flex items-center justify-center text-white text-xs font-bold border-2 border-white dark:border-slate-800 cursor-pointer hover:scale-110 transition-transform"
            style={{ backgroundColor: user.color }}
            title={user.userName}
          >
            {user.userName.charAt(0).toUpperCase()}
          </div>
        ))}
        {hiddenCount > 0 && (
          <div className="w-8 h-8 rounded-full flex items-center justify-center text-white text-xs font-bold bg-slate-400 border-2 border-white dark:border-slate-800">
            +{hiddenCount}
          </div>
        )}
      </div>
    </div>
  );
};

/**
 * Collaboration Toolbar Component
 * Shows session info and collaboration controls
 */
interface CollaborationToolbarProps {
  onLeaveSession?: () => void;
}

export const CollaborationToolbar: React.FC<CollaborationToolbarProps> = ({
  onLeaveSession,
}) => {
  const { session, isConnected, leaveSession } = useCollaboration();

  const handleLeave = () => {
    leaveSession();
    onLeaveSession?.();
  };

  return (
    <div className="flex items-center justify-between gap-4 p-3 bg-slate-50 dark:bg-slate-900 border-b border-slate-200 dark:border-slate-700 rounded-t-lg">
      <div className="flex items-center gap-3">
        <div
          className={`w-2 h-2 rounded-full ${
            isConnected ? 'bg-green-500' : 'bg-red-500'
          }`}
        />
        <span className="text-sm font-medium text-slate-700 dark:text-slate-300">
          {session?.title || 'Untitled Session'}
        </span>
        <span className="text-xs text-slate-500 dark:text-slate-400">
          {isConnected ? 'Connected' : 'Disconnected'}
        </span>
      </div>

      <div className="flex items-center gap-4">
        <PresenceIndicator />
        <button
          onClick={handleLeave}
          className="px-3 py-1 text-sm font-medium text-slate-600 dark:text-slate-400 hover:text-slate-900 dark:hover:text-white transition-colors"
        >
          Leave
        </button>
      </div>
    </div>
  );
};
