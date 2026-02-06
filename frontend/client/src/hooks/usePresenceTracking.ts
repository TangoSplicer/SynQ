/**
 * usePresenceTracking Hook
 * Tracks cursor position and selection, with throttling to reduce network traffic
 */

import { useEffect, useRef, useCallback } from 'react';
import { useCollaboration } from '@/contexts/CollaborationContext';

interface UsePresenceTrackingOptions {
  throttleMs?: number;
  editorRef?: React.RefObject<HTMLTextAreaElement | HTMLDivElement>;
}

export const usePresenceTracking = (options: UsePresenceTrackingOptions = {}) => {
  const { throttleMs = 100 } = options;
  const { updatePresence } = useCollaboration();
  
  const lastUpdateRef = useRef<number>(0);
  const throttleTimerRef = useRef<NodeJS.Timeout | null>(null);

  const trackPresence = useCallback(
    (cursorPosition: number, selectionStart: number, selectionEnd: number) => {
      const now = Date.now();
      const timeSinceLastUpdate = now - lastUpdateRef.current;

      if (timeSinceLastUpdate >= throttleMs) {
        // Enough time has passed, send immediately
        updatePresence(cursorPosition, selectionStart, selectionEnd);
        lastUpdateRef.current = now;
      } else {
        // Schedule for later
        if (throttleTimerRef.current) {
          clearTimeout(throttleTimerRef.current);
        }

        throttleTimerRef.current = setTimeout(() => {
          updatePresence(cursorPosition, selectionStart, selectionEnd);
          lastUpdateRef.current = Date.now();
        }, throttleMs - timeSinceLastUpdate);
      }
    },
    [updatePresence, throttleMs]
  );

  // Cleanup timer on unmount
  useEffect(() => {
    return () => {
      if (throttleTimerRef.current) {
        clearTimeout(throttleTimerRef.current);
      }
    };
  }, []);

  return { trackPresence };
};

/**
 * Hook to track cursor position from textarea or contenteditable
 */
export const useCursorTracking = (editorRef: React.RefObject<HTMLTextAreaElement | HTMLDivElement>) => {
  const { trackPresence } = usePresenceTracking();

  const handleSelectionChange = useCallback(() => {
    if (!editorRef.current) {
      return;
    }

    let cursorPosition = 0;
    let selectionStart = 0;
    let selectionEnd = 0;

    if (editorRef.current instanceof HTMLTextAreaElement) {
      cursorPosition = editorRef.current.selectionStart;
      selectionStart = editorRef.current.selectionStart;
      selectionEnd = editorRef.current.selectionEnd;
    } else {
      // For contenteditable
      const selection = window.getSelection();
      if (selection && selection.rangeCount > 0) {
        const range = selection.getRangeAt(0);
        const preCaretRange = range.cloneRange();
        preCaretRange.selectNodeContents(editorRef.current);
        preCaretRange.setEnd(range.endContainer, range.endOffset);
        cursorPosition = preCaretRange.toString().length;
        selectionStart = cursorPosition;
        selectionEnd = cursorPosition;
      }
    }

    trackPresence(cursorPosition, selectionStart, selectionEnd);
  }, [editorRef, trackPresence]);

  useEffect(() => {
    if (!editorRef.current) {
      return;
    }

    editorRef.current.addEventListener('click', handleSelectionChange);
    editorRef.current.addEventListener('keyup', handleSelectionChange);
    editorRef.current.addEventListener('mouseup', handleSelectionChange);

    return () => {
      if (!editorRef.current) {
        return;
      }
      editorRef.current.removeEventListener('click', handleSelectionChange);
      editorRef.current.removeEventListener('keyup', handleSelectionChange);
      editorRef.current.removeEventListener('mouseup', handleSelectionChange);
    };
  }, [editorRef, handleSelectionChange]);
};
