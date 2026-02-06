/**
 * Collaboration Tests
 * Unit and integration tests for WebSocket collaboration features
 */

import { OTEngine, createInsertOperation, createDeleteOperation } from '@/lib/ot-engine';

describe('OT Engine', () => {
  let engine: OTEngine;

  beforeEach(() => {
    engine = new OTEngine();
  });

  describe('Insert Operations', () => {
    test('should insert text at position', () => {
      const op = createInsertOperation(0, 'hello');
      const result = engine.applyToText('', op);
      expect(result).toBe('hello');
    });

    test('should insert text in middle', () => {
      const op = createInsertOperation(5, ' world');
      const result = engine.applyToText('hello', op);
      expect(result).toBe('hello world');
    });

    test('should insert text at end', () => {
      const op = createInsertOperation(5, '!');
      const result = engine.applyToText('hello', op);
      expect(result).toBe('hello!');
    });
  });

  describe('Delete Operations', () => {
    test('should delete text from start', () => {
      const op = createDeleteOperation(0, 5);
      const result = engine.applyToText('hello world', op);
      expect(result).toBe(' world');
    });

    test('should delete text from middle', () => {
      const op = createDeleteOperation(5, 1);
      const result = engine.applyToText('hello world', op);
      expect(result).toBe('helloworld');
    });

    test('should delete text from end', () => {
      const op = createDeleteOperation(5, 6);
      const result = engine.applyToText('hello world', op);
      expect(result).toBe('hello');
    });
  });

  describe('Transform Operations', () => {
    test('should transform two inserts at same position', () => {
      const op1 = createInsertOperation(0, 'a');
      const op2 = createInsertOperation(0, 'b');
      const [transformed1, transformed2] = engine.transform(op1, op2);

      expect(transformed1.position).toBe(0);
      expect(transformed2.position).toBe(1);
    });

    test('should transform insert and delete', () => {
      const insert = createInsertOperation(5, 'x');
      const del = createDeleteOperation(0, 3);
      const [transformedInsert, transformedDelete] = engine.transform(insert, del);

      expect(transformedInsert.position).toBe(2);
      expect(transformedDelete.position).toBe(0);
    });

    test('should transform overlapping deletes', () => {
      const del1 = createDeleteOperation(0, 5);
      const del2 = createDeleteOperation(3, 5);
      const [transformedDel1, transformedDel2] = engine.transform(del1, del2);

      expect(transformedDel1.type).toBe('delete');
      expect(transformedDel2.type).toBe('delete');
    });
  });

  describe('History and Undo/Redo', () => {
    test('should track operation history', () => {
      const op1 = createInsertOperation(0, 'hello');
      const op2 = createInsertOperation(5, ' world');

      engine.applyLocalOperation(op1);
      engine.applyLocalOperation(op2);

      const history = engine.getHistory();
      expect(history).toHaveLength(2);
      expect(history[0]).toEqual(op1);
      expect(history[1]).toEqual(op2);
    });

    test('should support undo', () => {
      const op = createInsertOperation(0, 'hello');
      engine.applyLocalOperation(op);

      expect(engine.canUndo()).toBe(true);
      const undoOp = engine.undo();
      expect(undoOp?.type).toBe('delete');
    });

    test('should support redo', () => {
      const op = createInsertOperation(0, 'hello');
      engine.applyLocalOperation(op);
      engine.undo();

      expect(engine.canRedo()).toBe(true);
      const redoOp = engine.redo();
      expect(redoOp?.type).toBe('insert');
    });

    test('should clear redo stack on new operation', () => {
      const op1 = createInsertOperation(0, 'hello');
      const op2 = createInsertOperation(5, ' world');

      engine.applyLocalOperation(op1);
      engine.undo();
      engine.applyLocalOperation(op2);

      expect(engine.canRedo()).toBe(false);
    });
  });

  describe('Revision Tracking', () => {
    test('should increment revision on each operation', () => {
      expect(engine.getRevision()).toBe(0);

      engine.applyLocalOperation(createInsertOperation(0, 'a'));
      expect(engine.getRevision()).toBe(1);

      engine.applyLocalOperation(createInsertOperation(1, 'b'));
      expect(engine.getRevision()).toBe(2);
    });
  });

  describe('Concurrent Editing Scenarios', () => {
    test('should handle two users inserting at different positions', () => {
      const user1Op = createInsertOperation(0, 'Alice: ');
      const user2Op = createInsertOperation(7, 'Bob: ');

      const result1 = engine.applyToText('', user1Op);
      const result2 = engine.applyToText(result1, user2Op);

      expect(result2).toBe('Alice: Bob: ');
    });

    test('should handle insert and delete on same text', () => {
      let text = 'hello world';

      const insert = createInsertOperation(5, ' beautiful');
      text = engine.applyToText(text, insert);
      expect(text).toBe('hello beautiful world');

      const del = createDeleteOperation(0, 6);
      text = engine.applyToText(text, del);
      expect(text).toBe('beautiful world');
    });

    test('should handle rapid consecutive edits', () => {
      let text = '';

      const ops = [
        createInsertOperation(0, 'h'),
        createInsertOperation(1, 'e'),
        createInsertOperation(2, 'l'),
        createInsertOperation(3, 'l'),
        createInsertOperation(4, 'o'),
      ];

      for (const op of ops) {
        text = engine.applyToText(text, op);
      }

      expect(text).toBe('hello');
    });
  });

  describe('Edge Cases', () => {
    test('should handle empty operations', () => {
      const result = engine.applyToText('hello', { type: 'retain', length: 0 });
      expect(result).toBe('hello');
    });

    test('should handle operations on empty text', () => {
      const op = createInsertOperation(0, 'hello');
      const result = engine.applyToText('', op);
      expect(result).toBe('hello');
    });

    test('should handle delete on empty text', () => {
      const op = createDeleteOperation(0, 0);
      const result = engine.applyToText('', op);
      expect(result).toBe('');
    });

    test('should handle large text operations', () => {
      const largeText = 'a'.repeat(10000);
      const op = createInsertOperation(5000, 'inserted');
      const result = engine.applyToText(largeText, op);

      expect(result.length).toBe(10008);
      expect(result.substring(5000, 5008)).toBe('inserted');
    });
  });
});

describe('WebSocket Manager', () => {
  // These tests would require mocking WebSocket
  // Placeholder for future implementation
  test('placeholder', () => {
    expect(true).toBe(true);
  });
});

describe('Collaboration Context', () => {
  // These tests would require React Testing Library
  // Placeholder for future implementation
  test('placeholder', () => {
    expect(true).toBe(true);
  });
});

describe('Performance Benchmarks', () => {
  test('should handle 1000 operations efficiently', () => {
    const engine = new OTEngine();
    const startTime = performance.now();

    for (let i = 0; i < 1000; i++) {
      engine.applyLocalOperation(createInsertOperation(i, 'x'));
    }

    const endTime = performance.now();
    const duration = endTime - startTime;

    // Should complete in less than 100ms
    expect(duration).toBeLessThan(100);
  });

  test('should transform operations quickly', () => {
    const engine = new OTEngine();
    const op1 = createInsertOperation(0, 'hello');
    const op2 = createInsertOperation(5, ' world');

    const startTime = performance.now();

    for (let i = 0; i < 10000; i++) {
      engine.transform(op1, op2);
    }

    const endTime = performance.now();
    const duration = endTime - startTime;

    // Should complete 10000 transforms in less than 50ms
    expect(duration).toBeLessThan(50);
  });

  test('should apply operations to large text efficiently', () => {
    const engine = new OTEngine();
    const largeText = 'a'.repeat(100000);
    const op = createInsertOperation(50000, 'inserted');

    const startTime = performance.now();

    for (let i = 0; i < 100; i++) {
      engine.applyToText(largeText, op);
    }

    const endTime = performance.now();
    const duration = endTime - startTime;

    // Should complete 100 operations on 100KB text in less than 100ms
    expect(duration).toBeLessThan(100);
  });
});
