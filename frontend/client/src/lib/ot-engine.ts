/**
 * Operational Transformation (OT) Engine
 * Implements conflict-free concurrent editing with OT algorithm
 */

export type OperationType = 'insert' | 'delete' | 'retain';

export interface Operation {
  type: OperationType;
  position?: number;
  content?: string;
  length?: number;
}

export interface TransformedOperation {
  operation: Operation;
  revision: number;
}

/**
 * OT Engine for managing concurrent edits
 */
export class OTEngine {
  private history: Operation[] = [];
  private revision: number = 0;
  private pendingOperations: Operation[] = [];
  private undoStack: Operation[] = [];
  private redoStack: Operation[] = [];

  /**
   * Apply a local operation
   */
  public applyLocalOperation(operation: Operation): TransformedOperation {
    // Add to history
    this.history.push(operation);
    this.revision++;

    // Clear redo stack on new operation
    this.redoStack = [];

    // Add to undo stack
    this.undoStack.push(this.invertOperation(operation));

    return {
      operation,
      revision: this.revision,
    };
  }

  /**
   * Apply a remote operation
   */
  public applyRemoteOperation(remoteOp: Operation, remoteRevision: number): Operation {
    // Transform remote operation against all pending local operations
    let transformedOp = remoteOp;

    for (const pendingOp of this.pendingOperations) {
      transformedOp = this.transform(transformedOp, pendingOp)[0];
    }

    // Add to history
    this.history.push(transformedOp);
    this.revision++;

    return transformedOp;
  }

  /**
   * Transform two operations against each other
   * Returns [op1', op2'] where op1' is op1 transformed against op2
   */
  public transform(op1: Operation, op2: Operation): [Operation, Operation] {
    // If both operations are on the same position, apply priority
    if (op1.type === 'insert' && op2.type === 'insert') {
      if (op1.position === op2.position) {
        // Priority: op1 comes first if position is same
        return [
          op1,
          { type: 'insert', position: (op2.position || 0) + (op1.content?.length || 0), content: op2.content },
        ];
      } else if ((op1.position || 0) < (op2.position || 0)) {
        return [op1, { type: 'insert', position: (op2.position || 0) + (op1.content?.length || 0), content: op2.content }];
      } else {
        return [{ type: 'insert', position: (op1.position || 0) + (op2.content?.length || 0), content: op1.content }, op2];
      }
    }

    if (op1.type === 'insert' && op2.type === 'delete') {
      const insertPos = op1.position || 0;
      const deletePos = op2.position || 0;
      const deleteLen = op2.length || 0;

      if (insertPos < deletePos) {
        return [op1, { type: 'delete', position: deletePos + (op1.content?.length || 0), length: deleteLen }];
      } else if (insertPos >= deletePos + deleteLen) {
        return [{ type: 'insert', position: insertPos - deleteLen, content: op1.content }, op2];
      } else {
        return [op1, { type: 'delete', position: deletePos, length: deleteLen }];
      }
    }

    if (op1.type === 'delete' && op2.type === 'insert') {
      const deletePos = op1.position || 0;
      const deleteLen = op1.length || 0;
      const insertPos = op2.position || 0;

      if (deletePos + deleteLen <= insertPos) {
        return [op1, { type: 'insert', position: insertPos - deleteLen, content: op2.content }];
      } else if (deletePos >= insertPos) {
        return [{ type: 'delete', position: deletePos + (op2.content?.length || 0), length: deleteLen }, op2];
      } else {
        return [op1, { type: 'insert', position: insertPos, content: op2.content }];
      }
    }

    if (op1.type === 'delete' && op2.type === 'delete') {
      const deletePos1 = op1.position || 0;
      const deleteLen1 = op1.length || 0;
      const deletePos2 = op2.position || 0;
      const deleteLen2 = op2.length || 0;

      if (deletePos1 + deleteLen1 <= deletePos2) {
        return [op1, { type: 'delete', position: deletePos2 - deleteLen1, length: deleteLen2 }];
      } else if (deletePos2 + deleteLen2 <= deletePos1) {
        return [{ type: 'delete', position: deletePos1 - deleteLen2, length: deleteLen1 }, op2];
      } else {
        // Overlapping deletes - adjust positions
        const newPos1 = Math.min(deletePos1, deletePos2);
        const newLen1 = Math.max(deletePos1 + deleteLen1, deletePos2 + deleteLen2) - newPos1;
        return [
          { type: 'delete', position: newPos1, length: newLen1 },
          { type: 'delete', position: newPos1, length: 0 },
        ];
      }
    }

    // Default: return operations unchanged
    return [op1, op2];
  }

  /**
   * Compose two operations into one
   */
  public compose(op1: Operation, op2: Operation): Operation {
    if (op1.type === 'insert' && op2.type === 'insert') {
      if ((op1.position || 0) + (op1.content?.length || 0) === (op2.position || 0)) {
        return {
          type: 'insert',
          position: op1.position,
          content: (op1.content || '') + (op2.content || ''),
        };
      }
    }

    if (op1.type === 'delete' && op2.type === 'delete') {
      if (op1.position === op2.position) {
        return {
          type: 'delete',
          position: op1.position,
          length: (op1.length || 0) + (op2.length || 0),
        };
      }
    }

    // Can't compose, return first operation
    return op1;
  }

  /**
   * Invert an operation (for undo)
   */
  public invertOperation(operation: Operation): Operation {
    if (operation.type === 'insert') {
      return {
        type: 'delete',
        position: operation.position,
        length: operation.content?.length || 0,
      };
    } else if (operation.type === 'delete') {
      return {
        type: 'insert',
        position: operation.position,
        content: '', // We don't store the deleted content here
      };
    }
    return operation;
  }

  /**
   * Apply operation to text
   */
  public applyToText(text: string, operation: Operation): string {
    if (operation.type === 'insert') {
      const pos = operation.position || 0;
      return text.slice(0, pos) + (operation.content || '') + text.slice(pos);
    } else if (operation.type === 'delete') {
      const pos = operation.position || 0;
      const len = operation.length || 0;
      return text.slice(0, pos) + text.slice(pos + len);
    }
    return text;
  }

  /**
   * Get operation history
   */
  public getHistory(): Operation[] {
    return [...this.history];
  }

  /**
   * Get current revision
   */
  public getRevision(): number {
    return this.revision;
  }

  /**
   * Add pending operation
   */
  public addPendingOperation(operation: Operation): void {
    this.pendingOperations.push(operation);
  }

  /**
   * Clear pending operations
   */
  public clearPendingOperations(): void {
    this.pendingOperations = [];
  }

  /**
   * Undo last operation
   */
  public undo(): Operation | null {
    if (this.undoStack.length === 0) {
      return null;
    }

    const undoOp = this.undoStack.pop()!;
    this.redoStack.push(undoOp);
    return undoOp;
  }

  /**
   * Redo last undone operation
   */
  public redo(): Operation | null {
    if (this.redoStack.length === 0) {
      return null;
    }

    const redoOp = this.redoStack.pop()!;
    this.undoStack.push(redoOp);
    return redoOp;
  }

  /**
   * Can undo
   */
  public canUndo(): boolean {
    return this.undoStack.length > 0;
  }

  /**
   * Can redo
   */
  public canRedo(): boolean {
    return this.redoStack.length > 0;
  }
}

/**
 * Helper function to create insert operation
 */
export function createInsertOperation(position: number, content: string): Operation {
  return {
    type: 'insert',
    position,
    content,
  };
}

/**
 * Helper function to create delete operation
 */
export function createDeleteOperation(position: number, length: number): Operation {
  return {
    type: 'delete',
    position,
    length,
  };
}

/**
 * Helper function to create retain operation
 */
export function createRetainOperation(length: number): Operation {
  return {
    type: 'retain',
    length,
  };
}

/**
 * Serialize operation to JSON
 */
export function serializeOperation(operation: Operation): string {
  return JSON.stringify(operation);
}

/**
 * Deserialize operation from JSON
 */
export function deserializeOperation(json: string): Operation {
  return JSON.parse(json);
}
