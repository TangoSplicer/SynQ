import { afterEach, beforeEach, describe, expect, it, vi } from 'vitest';
import GraphQLClient from '@/lib/graphql-client';
import SecureWebSocketClient from '@/lib/websocket-secure';

class MockWebSocket {
  static readonly CONNECTING = 0;
  static readonly OPEN = 1;
  static readonly CLOSED = 3;
  static instances: MockWebSocket[] = [];

  readyState = MockWebSocket.CONNECTING;
  sent: string[] = [];
  onopen: ((event: Event) => void) | null = null;
  onmessage: ((event: MessageEvent) => void) | null = null;
  onerror: ((event: Event) => void) | null = null;
  onclose: ((event: CloseEvent) => void) | null = null;

  constructor(readonly url: string) {
    MockWebSocket.instances.push(this);
    queueMicrotask(() => {
      this.readyState = MockWebSocket.OPEN;
      this.onopen?.({} as Event);
    });
  }

  send(message: string): void {
    this.sent.push(message);
  }

  close(): void {
    this.readyState = MockWebSocket.CLOSED;
  }

  receive(message: Record<string, unknown>): void {
    this.onmessage?.({ data: JSON.stringify(message) } as MessageEvent);
  }
}

const jsonResponse = (body: unknown, status = 200): Response =>
  new Response(JSON.stringify(body), {
    status,
    headers: { 'Content-Type': 'application/json' },
  });

describe('Realtime client local fixtures', () => {
  beforeEach(() => {
    MockWebSocket.instances = [];
    vi.stubGlobal('WebSocket', MockWebSocket);
  });

  afterEach(() => {
    vi.unstubAllGlobals();
    vi.restoreAllMocks();
  });

  const createSocketClient = () => new SecureWebSocketClient({
    url: 'ws://local.test/session',
    token: 'test-token',
    sessionId: 'session-1',
    heartbeatInterval: 60_000,
  });

  const authenticate = async (client: SecureWebSocketClient): Promise<MockWebSocket> => {
    await client.connect();
    const socket = MockWebSocket.instances[0];
    socket.receive({
      type: 'authenticated',
      sessionId: 'session-1',
      userId: 'user-1',
      timestamp: new Date().toISOString(),
    });
    return socket;
  };

  it('opens a local socket and sends its authentication envelope', async () => {
    const client = createSocketClient();
    await client.connect();

    const socket = MockWebSocket.instances[0];
    expect(socket.url).toBe('ws://local.test/session');
    expect(client.getStatus()).toMatchObject({ connected: true, authenticated: false });
    expect(JSON.parse(socket.sent[0])).toMatchObject({
      type: 'authenticate',
      sessionId: 'session-1',
      token: 'test-token',
    });
    client.disconnect();
  });

  it('becomes ready after a local authentication response and dispatches a message handler', async () => {
    const client = createSocketClient();
    const handler = vi.fn();
    client.on('edit', handler);
    const socket = await authenticate(client);

    socket.receive({
      type: 'edit',
      sessionId: 'session-1',
      userId: 'user-1',
      content: 'local edit',
      timestamp: new Date().toISOString(),
    });

    expect(client.isReady()).toBe(true);
    expect(handler).toHaveBeenCalledWith(expect.objectContaining({ type: 'edit', content: 'local edit' }));
    expect(client.getStatus().stats.messagesReceived).toBe(2);
    client.disconnect();
  });

  it('queues a message before authentication and drains it after authentication succeeds', async () => {
    const client = createSocketClient();
    expect(await client.send({ type: 'edit', content: 'queued edit' })).toBe(false);

    const socket = await authenticate(client);
    const sentMessages = socket.sent.map((message) => JSON.parse(message));
    expect(sentMessages).toEqual(expect.arrayContaining([
      expect.objectContaining({ type: 'authenticate' }),
      expect.objectContaining({ type: 'edit', content: 'queued edit', sessionId: 'session-1' }),
    ]));
    client.disconnect();
  });

  it('stops dispatching a handler after it is removed', async () => {
    const client = createSocketClient();
    const handler = vi.fn();
    client.on('comment', handler);
    client.off('comment', handler);
    const socket = await authenticate(client);

    socket.receive({ type: 'comment', sessionId: 'session-1', userId: 'user-1', timestamp: new Date().toISOString() });
    expect(handler).not.toHaveBeenCalled();
    client.disconnect();
  });
});

describe('GraphQL client local fixtures', () => {
  let fetchMock: ReturnType<typeof vi.fn>;
  let client: GraphQLClient;

  beforeEach(() => {
    fetchMock = vi.fn();
    vi.stubGlobal('fetch', fetchMock);
    client = new GraphQLClient('https://api.local.test/graphql', 'test-token');
  });

  afterEach(() => {
    vi.unstubAllGlobals();
    vi.restoreAllMocks();
  });

  it('sends GraphQL requests with bearer authentication and variables', async () => {
    fetchMock.mockResolvedValueOnce(jsonResponse({ data: { circuit: { id: 'c-1', name: 'Fixture', qubits: 2 } } }));

    const result = await client.getCircuit('c-1');
    expect(result).toMatchObject({ id: 'c-1', name: 'Fixture', qubits: 2 });
    expect(fetchMock).toHaveBeenCalledWith('https://api.local.test/graphql', expect.objectContaining({ method: 'POST' }));
    const request = fetchMock.mock.calls[0][1] as RequestInit;
    expect(request.headers).toMatchObject({ Authorization: 'Bearer test-token' });
    expect(JSON.parse(String(request.body))).toMatchObject({ variables: { id: 'c-1' } });
  });

  it('returns typed list and mutation data from mocked responses', async () => {
    fetchMock
      .mockResolvedValueOnce(jsonResponse({ data: { circuits: [{ id: 'c-1', name: 'Fixture', qubits: 2 }] } }))
      .mockResolvedValueOnce(jsonResponse({ data: { createCircuit: { id: 'c-2', name: 'Created', qubits: 3 } } }));

    await expect(client.listCircuits(5, 1)).resolves.toEqual([expect.objectContaining({ id: 'c-1', qubits: 2 })]);
    await expect(client.createCircuit('Created', 3)).resolves.toEqual(expect.objectContaining({ id: 'c-2', name: 'Created' }));
  });

  it('returns empty or null fallbacks for GraphQL errors without a backend service', async () => {
    fetchMock
      .mockResolvedValueOnce(jsonResponse({ errors: [{ message: 'Unavailable' }] }))
      .mockResolvedValueOnce(jsonResponse({ errors: [{ message: 'Unavailable' }] }));

    await expect(client.listJobs('completed')).resolves.toEqual([]);
    await expect(client.addComment('session-1', 2, 'note')).resolves.toBeNull();
  });

  it('converts network failure into a query error and high-level fallback', async () => {
    fetchMock.mockRejectedValueOnce(new Error('network unavailable'));
    await expect(client.listMLModels()).resolves.toEqual([]);
  });
});
