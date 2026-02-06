/**
 * useOTEngine Hook
 * Manages operational transformation for collaborative editing
 */

import { useCallback, useRef, useState } from 'react';
import { OTEngine, Operation } from '@/lib/ot-engine';
import { useCollaboration } from '@/contexts/CollaborationContext';

export const useOTEngine = () => {
  const engineRef = useRef(new OTEngine());
  const [revision, setRevision] = useState(0);
  const [canUndo, setCanUndo] = useState(false);
  const [canRedo, setCanRedo] = useState(false);
  const { sendEdit, onMessage } = useCollaboration();

  // Apply local operation
  const applyLocalOperation = useCallback(
    (operation: Operation) => {
      const result = engineRef.current.applyLocalOperation(operation);
      setRevision(result.revision);
      setCanUndo(engineRef.current.canUndo());
      setCanRedo(engineRef.current.canRedo());

      // Send to server
      sendEdit(operation);

      return result;
    },
    [sendEdit]
  );

  // Apply remote operation
  const applyRemoteOperation = useCallback(
    (remoteOp: Operation, remoteRevision: number) => {
      const transformedOp = engineRef.current.applyRemoteOperation(remoteOp, remoteRevision);
      setRevision(engineRef.current.getRevision());
      return transformedOp;
    },
    []
  );

  // Transform operations
  const transform = useCallback((op1: Operation, op2: Operation) => {
    return engineRef.current.transform(op1, op2);
  }, []);

  // Apply operation to text
  const applyToText = useCallback((text: string, operation: Operation) => {
    return engineRef.current.applyToText(text, operation);
  }, []);

  // Undo
  const undo = useCallback(() => {
    const undoOp = engineRef.current.undo();
    setCanUndo(engineRef.current.canUndo());
    setCanRedo(engineRef.current.canRedo());
    return undoOp;
  }, []);

  // Redo
  const redo = useCallback(() => {
    const redoOp = engineRef.current.redo();
    setCanUndo(engineRef.current.canUndo());
    setCanRedo(engineRef.current.canRedo());
    return redoOp;
  }, []);

  // Get history
  const getHistory = useCallback(() => {
    return engineRef.current.getHistory();
  }, []);

  // Register remote edit handler
  onMessage('edit', (message) => {
    if (message.data.operation) {
      applyRemoteOperation(message.data.operation, message.revision || 0);
    }
  });

  return {
    applyLocalOperation,
    applyRemoteOperation,
    transform,
    applyToText,
    undo,
    redo,
    canUndo,
    canRedo,
    getHistory,
    revision,
  };
};
