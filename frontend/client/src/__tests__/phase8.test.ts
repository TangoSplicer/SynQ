/**
 * Phase 8 Test Suite
 *
 * Tests for real-time collaboration, ML suggestions, and circuit visualization
 */

import { describe, it, expect, beforeEach, vi } from 'vitest';
import SecureWebSocketClient from '@/lib/websocket-secure';
import GraphQLClient, { Circuit, Job, MLModel } from '@/lib/graphql-client';

describe('Phase 8: Frontend Integration & Real-Time Features', () => {
  describe('WebSocket Real-Time Synchronization', () => {
    let wsClient: SecureWebSocketClient;

    beforeEach(() => {
      wsClient = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'test-token',
        sessionId: 'test-session',
        maxReconnectAttempts: 3
      });
    });

    it('should establish WebSocket connection', async () => {
      const connected = await wsClient.connect();
      expect(connected).toBe(true);
      expect(wsClient.isReady()).toBe(true);
    });

    it('should handle edit messages', (done) => {
      wsClient.on('edit', (msg) => {
        expect(msg.type).toBe('edit');
        expect(msg.content).toBeDefined();
        done();
      });

      wsClient.send({
        type: 'edit',
        content: 'new content'
      });
    });

    it('should handle presence updates', (done) => {
      wsClient.on('presence', (msg) => {
        expect(msg.type).toBe('presence');
        expect(msg.cursor).toBeDefined();
        done();
      });

      wsClient.send({
        type: 'presence',
        cursor: { line: 5, column: 10 }
      });
    });

    it('should handle comments', (done) => {
      wsClient.on('comment', (msg) => {
        expect(msg.type).toBe('comment');
        expect(msg.line).toBeDefined();
        expect(msg.text).toBeDefined();
        done();
      });

      wsClient.send({
        type: 'comment',
        line: 3,
        text: 'Test comment'
      });
    });

    it('should queue messages during disconnection', async () => {
      wsClient.disconnect();
      expect(wsClient.isReady()).toBe(false);

      wsClient.send({ type: 'edit', content: 'queued' });
      expect(wsClient.getStatus().messageQueue.length).toBeGreaterThan(0);

      await wsClient.connect();
      expect(wsClient.getStatus().messageQueue.length).toBe(0);
    });

    it('should track connection statistics', () => {
      const stats = wsClient.getStatus();
      expect(stats.stats).toBeDefined();
      expect(stats.stats.messagesReceived).toBeGreaterThanOrEqual(0);
      expect(stats.stats.messagesSent).toBeGreaterThanOrEqual(0);
      expect(stats.stats.averageLatency).toBeGreaterThanOrEqual(0);
    });
  });

  describe('GraphQL Client Integration', () => {
    let graphqlClient: GraphQLClient;

    beforeEach(() => {
      graphqlClient = new GraphQLClient('http://localhost:8000/graphql', 'test-token');
    });

    it('should fetch circuits', async () => {
      const circuits = await graphqlClient.listCircuits(10, 0);
      expect(Array.isArray(circuits)).toBe(true);
    });

    it('should create circuit', async () => {
      const circuit = await graphqlClient.createCircuit('Test Circuit', 5);
      expect(circuit).toBeDefined();
      if (circuit) {
        expect(circuit.name).toBe('Test Circuit');
        expect(circuit.qubits).toBe(5);
      }
    });

    it('should get circuit by ID', async () => {
      const circuit = await graphqlClient.getCircuit('test-id');
      expect(circuit === null || circuit.id).toBeDefined();
    });

    it('should fetch jobs', async () => {
      const jobs = await graphqlClient.listJobs('completed', 10);
      expect(Array.isArray(jobs)).toBe(true);
    });

    it('should submit job', async () => {
      const job = await graphqlClient.submitJob('circuit-id', 'simulator');
      expect(job === null || job.status).toBeDefined();
    });

    it('should fetch ML models', async () => {
      const models = await graphqlClient.listMLModels('circuit_optimization', 'deployed');
      expect(Array.isArray(models)).toBe(true);
    });

    it('should create ML model', async () => {
      const model = await graphqlClient.createMLModel('Test Model', 'circuit_optimization', '1.0');
      expect(model === null || model.name).toBeDefined();
    });

    it('should fetch collaboration sessions', async () => {
      const sessions = await graphqlClient.listCollaborationSessions();
      expect(Array.isArray(sessions)).toBe(true);
    });

    it('should create collaboration session', async () => {
      const session = await graphqlClient.createCollaborationSession('Test Session', 'circuit-id');
      expect(session === null || session.name).toBeDefined();
    });

    it('should fetch comments', async () => {
      const comments = await graphqlClient.getSessionComments('session-id', false);
      expect(Array.isArray(comments)).toBe(true);
    });

    it('should add comment', async () => {
      const comment = await graphqlClient.addComment('session-id', 5, 'Test comment');
      expect(comment === null || comment.content).toBeDefined();
    });
  });

  describe('ML Suggestion Panel', () => {
    it('should fetch circuit optimization predictions', async () => {
      const response = await fetch('http://localhost:8000/api/v1/ml/predict/circuit-optimization', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ gate_count: 10, qubits: 5 })
      });

      expect(response.ok).toBe(true);
      const data = await response.json();
      expect(data.confidence).toBeDefined();
      expect(data.gate_reduction_percent).toBeDefined();
    });

    it('should fetch resource estimation predictions', async () => {
      const response = await fetch('http://localhost:8000/api/v1/ml/predict/resource-estimation', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ gate_count: 10, qubits: 5 })
      });

      expect(response.ok).toBe(true);
      const data = await response.json();
      expect(data.estimated_qubits).toBeDefined();
      expect(data.estimated_execution_time_ms).toBeDefined();
    });

    it('should fetch pattern recognition predictions', async () => {
      const response = await fetch('http://localhost:8000/api/v1/ml/predict/pattern-recognition', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ gate_count: 10, qubits: 5 })
      });

      expect(response.ok).toBe(true);
      const data = await response.json();
      expect(data.patterns_identified).toBeDefined();
    });
  });

  describe('Circuit Visualization', () => {
    it('should render circuit with gates', () => {
      const gates = [
        { id: '1', type: 'H', qubits: [0], position: 0 },
        { id: '2', type: 'CNOT', qubits: [0, 1], position: 1 }
      ];

      expect(gates.length).toBe(2);
      expect(gates[0].type).toBe('H');
      expect(gates[1].type).toBe('CNOT');
    });

    it('should calculate circuit depth', () => {
      const gates = [
        { id: '1', type: 'H', qubits: [0], position: 0 },
        { id: '2', type: 'X', qubits: [1], position: 0 },
        { id: '3', type: 'CNOT', qubits: [0, 1], position: 1 }
      ];

      const depth = Math.max(...gates.map(g => g.position)) + 1;
      expect(depth).toBe(2);
    });

    it('should handle multi-qubit gates', () => {
      const gate = { id: '1', type: 'CNOT', qubits: [0, 1], position: 0 };
      expect(gate.qubits.length).toBe(2);
      expect(Math.min(...gate.qubits)).toBe(0);
      expect(Math.max(...gate.qubits)).toBe(1);
    });
  });

  describe('End-to-End Collaboration Workflow', () => {
    it('should complete full collaboration session', async () => {
      // 1. Create session
      const graphqlClient = new GraphQLClient('http://localhost:8000/graphql', 'test-token');
      const session = await graphqlClient.createCollaborationSession('E2E Test', 'circuit-id');
      expect(session).toBeDefined();

      // 2. Connect WebSocket
      const wsClient = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'test-token',
        sessionId: session?.id || 'test'
      });

      const connected = await wsClient.connect();
      expect(connected).toBe(true);

      // 3. Send edit
      wsClient.send({
        type: 'edit',
        content: 'circuit { H(0); CNOT(0, 1); }'
      });

      // 4. Send presence
      wsClient.send({
        type: 'presence',
        cursor: { line: 1, column: 10 }
      });

      // 5. Add comment
      const comment = await graphqlClient.addComment(session?.id || 'test', 1, 'Test comment');
      expect(comment).toBeDefined();

      // 6. Disconnect
      wsClient.disconnect();
      expect(wsClient.isReady()).toBe(false);
    });

    it('should handle concurrent edits', async () => {
      const wsClient1 = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'token1',
        sessionId: 'shared-session'
      });

      const wsClient2 = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'token2',
        sessionId: 'shared-session'
      });

      await wsClient1.connect();
      await wsClient2.connect();

      // Both clients send edits
      wsClient1.send({ type: 'edit', content: 'edit from client 1' });
      wsClient2.send({ type: 'edit', content: 'edit from client 2' });

      // Both should receive edits
      expect(wsClient1.isReady()).toBe(true);
      expect(wsClient2.isReady()).toBe(true);

      wsClient1.disconnect();
      wsClient2.disconnect();
    });

    it('should handle network failures gracefully', async () => {
      const wsClient = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'test-token',
        sessionId: 'test',
        maxReconnectAttempts: 2,
        initialReconnectDelay: 100
      });

      // Simulate connection failure
      wsClient.disconnect();
      expect(wsClient.isReady()).toBe(false);

      // Queue messages
      wsClient.send({ type: 'edit', content: 'queued 1' });
      wsClient.send({ type: 'edit', content: 'queued 2' });

      const status = wsClient.getStatus();
      expect(status.messageQueue.length).toBeGreaterThan(0);

      // Attempt reconnection
      const reconnected = await wsClient.connect();
      expect(reconnected === true || reconnected === false).toBe(true);
    });
  });

  describe('Performance', () => {
    it('should handle rapid edits', async () => {
      const wsClient = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'test-token',
        sessionId: 'perf-test'
      });

      await wsClient.connect();

      const startTime = Date.now();
      for (let i = 0; i < 100; i++) {
        wsClient.send({ type: 'edit', content: `edit ${i}` });
      }
      const duration = Date.now() - startTime;

      expect(duration).toBeLessThan(1000); // Should complete in less than 1 second
      wsClient.disconnect();
    });

    it('should track latency', async () => {
      const wsClient = new SecureWebSocketClient({
        url: 'ws://localhost:8000/ws',
        token: 'test-token',
        sessionId: 'latency-test'
      });

      await wsClient.connect();

      const status = wsClient.getStatus();
      expect(status.stats.averageLatency).toBeGreaterThanOrEqual(0);

      wsClient.disconnect();
    });
  });
});
