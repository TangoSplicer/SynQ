/**
 * GraphQL Client
 *
 * Provides type-safe GraphQL queries and mutations
 * for fetching circuits, jobs, models, and collaboration data
 */

import { useState, useCallback } from 'react';

export interface GraphQLError {
  message: string;
  locations?: Array<{ line: number; column: number }>;
  path?: string[];
}

export interface GraphQLResponse<T> {
  data?: T;
  errors?: GraphQLError[];
}

export interface Circuit {
  id: string;
  name: string;
  qubits: number;
  gates: Array<{
    type: string;
    qubits: number[];
    params?: Record<string, number>;
  }>;
  createdAt: string;
  updatedAt: string;
}

export interface Job {
  id: string;
  circuitId: string;
  backend: string;
  status: 'pending' | 'running' | 'completed' | 'failed';
  result?: Record<string, any>;
  createdAt: string;
  completedAt?: string;
}

export interface MLModel {
  id: string;
  name: string;
  type: 'circuit_optimization' | 'resource_estimation' | 'pattern_recognition';
  version: string;
  status: 'training' | 'deployed' | 'archived';
  accuracy?: number;
  createdAt: string;
}

export interface TrainingJob {
  id: string;
  modelId: string;
  status: 'pending' | 'running' | 'completed' | 'failed';
  epochs: number;
  batchSize: number;
  accuracy?: number;
  loss?: number;
  startedAt: string;
  completedAt?: string;
}

export interface CollaborationSession {
  id: string;
  name: string;
  circuitId: string;
  participants: Array<{
    userId: string;
    name: string;
    role: 'owner' | 'editor' | 'viewer';
    joinedAt: string;
  }>;
  createdAt: string;
}

export interface Comment {
  id: string;
  sessionId: string;
  line: number;
  author: string;
  content: string;
  resolved: boolean;
  createdAt: string;
  replies: Comment[];
}

class GraphQLClient {
  private url: string;
  private token: string;

  constructor(url: string, token: string) {
    this.url = url;
    this.token = token;
  }

  async query<T>(query: string, variables?: Record<string, any>): Promise<GraphQLResponse<T>> {
    try {
      const response = await fetch(this.url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
          'Authorization': `Bearer ${this.token}`
        },
        body: JSON.stringify({ query, variables })
      });

      if (!response.ok) {
        throw new Error(`HTTP ${response.status}: ${response.statusText}`);
      }

      return await response.json();
    } catch (error) {
      return {
        errors: [
          {
            message: error instanceof Error ? error.message : 'Unknown error'
          }
        ]
      };
    }
  }

  async mutation<T>(mutation: string, variables?: Record<string, any>): Promise<GraphQLResponse<T>> {
    return this.query<T>(mutation, variables);
  }

  // Circuit queries
  async getCircuit(id: string): Promise<Circuit | null> {
    const query = `
      query GetCircuit($id: ID!) {
        circuit(id: $id) {
          id
          name
          qubits
          gates {
            type
            qubits
            params
          }
          createdAt
          updatedAt
        }
      }
    `;

    const response = await this.query<{ circuit: Circuit }>(query, { id });
    return response.data?.circuit || null;
  }

  async listCircuits(limit: number = 10, offset: number = 0): Promise<Circuit[]> {
    const query = `
      query ListCircuits($limit: Int!, $offset: Int!) {
        circuits(limit: $limit, offset: $offset) {
          id
          name
          qubits
          createdAt
          updatedAt
        }
      }
    `;

    const response = await this.query<{ circuits: Circuit[] }>(query, { limit, offset });
    return response.data?.circuits || [];
  }

  async createCircuit(name: string, qubits: number): Promise<Circuit | null> {
    const mutation = `
      mutation CreateCircuit($name: String!, $qubits: Int!) {
        createCircuit(name: $name, qubits: $qubits) {
          id
          name
          qubits
          createdAt
        }
      }
    `;

    const response = await this.mutation<{ createCircuit: Circuit }>(mutation, { name, qubits });
    return response.data?.createCircuit || null;
  }

  // Job queries
  async getJob(id: string): Promise<Job | null> {
    const query = `
      query GetJob($id: ID!) {
        job(id: $id) {
          id
          circuitId
          backend
          status
          result
          createdAt
          completedAt
        }
      }
    `;

    const response = await this.query<{ job: Job }>(query, { id });
    return response.data?.job || null;
  }

  async listJobs(status?: string, limit: number = 10): Promise<Job[]> {
    const query = `
      query ListJobs($status: String, $limit: Int!) {
        jobs(status: $status, limit: $limit) {
          id
          circuitId
          backend
          status
          createdAt
        }
      }
    `;

    const response = await this.query<{ jobs: Job[] }>(query, { status, limit });
    return response.data?.jobs || [];
  }

  async submitJob(circuitId: string, backend: string): Promise<Job | null> {
    const mutation = `
      mutation SubmitJob($circuitId: ID!, $backend: String!) {
        submitJob(circuitId: $circuitId, backend: $backend) {
          id
          circuitId
          backend
          status
          createdAt
        }
      }
    `;

    const response = await this.mutation<{ submitJob: Job }>(mutation, { circuitId, backend });
    return response.data?.submitJob || null;
  }

  // ML Model queries
  async getMLModel(id: string): Promise<MLModel | null> {
    const query = `
      query GetMLModel($id: ID!) {
        mlModel(id: $id) {
          id
          name
          type
          version
          status
          accuracy
          createdAt
        }
      }
    `;

    const response = await this.query<{ mlModel: MLModel }>(query, { id });
    return response.data?.mlModel || null;
  }

  async listMLModels(type?: string, status?: string): Promise<MLModel[]> {
    const query = `
      query ListMLModels($type: String, $status: String) {
        mlModels(type: $type, status: $status) {
          id
          name
          type
          version
          status
          accuracy
          createdAt
        }
      }
    `;

    const response = await this.query<{ mlModels: MLModel[] }>(query, { type, status });
    return response.data?.mlModels || [];
  }

  async createMLModel(name: string, type: string, version: string): Promise<MLModel | null> {
    const mutation = `
      mutation CreateMLModel($name: String!, $type: String!, $version: String!) {
        createMLModel(name: $name, type: $type, version: $version) {
          id
          name
          type
          version
          status
          createdAt
        }
      }
    `;

    const response = await this.mutation<{ createMLModel: MLModel }>(mutation, { name, type, version });
    return response.data?.createMLModel || null;
  }

  // Collaboration queries
  async getCollaborationSession(id: string): Promise<CollaborationSession | null> {
    const query = `
      query GetSession($id: ID!) {
        collaborationSession(id: $id) {
          id
          name
          circuitId
          participants {
            userId
            name
            role
            joinedAt
          }
          createdAt
        }
      }
    `;

    const response = await this.query<{ collaborationSession: CollaborationSession }>(query, { id });
    return response.data?.collaborationSession || null;
  }

  async listCollaborationSessions(): Promise<CollaborationSession[]> {
    const query = `
      query ListSessions {
        collaborationSessions {
          id
          name
          circuitId
          participants {
            userId
            name
            role
          }
          createdAt
        }
      }
    `;

    const response = await this.query<{ collaborationSessions: CollaborationSession[] }>(query);
    return response.data?.collaborationSessions || [];
  }

  async createCollaborationSession(name: string, circuitId: string): Promise<CollaborationSession | null> {
    const mutation = `
      mutation CreateSession($name: String!, $circuitId: ID!) {
        createCollaborationSession(name: $name, circuitId: $circuitId) {
          id
          name
          circuitId
          participants {
            userId
            name
            role
          }
          createdAt
        }
      }
    `;

    const response = await this.mutation<{ createCollaborationSession: CollaborationSession }>(
      mutation,
      { name, circuitId }
    );
    return response.data?.createCollaborationSession || null;
  }

  // Comment queries
  async getSessionComments(sessionId: string, resolved?: boolean): Promise<Comment[]> {
    const query = `
      query GetComments($sessionId: ID!, $resolved: Boolean) {
        sessionComments(sessionId: $sessionId, resolved: $resolved) {
          id
          sessionId
          line
          author
          content
          resolved
          createdAt
          replies {
            id
            author
            content
            createdAt
          }
        }
      }
    `;

    const response = await this.query<{ sessionComments: Comment[] }>(query, { sessionId, resolved });
    return response.data?.sessionComments || [];
  }

  async addComment(sessionId: string, line: number, content: string): Promise<Comment | null> {
    const mutation = `
      mutation AddComment($sessionId: ID!, $line: Int!, $content: String!) {
        addComment(sessionId: $sessionId, line: $line, content: $content) {
          id
          sessionId
          line
          author
          content
          resolved
          createdAt
        }
      }
    `;

    const response = await this.mutation<{ addComment: Comment }>(mutation, { sessionId, line, content });
    return response.data?.addComment || null;
  }
}

export default GraphQLClient;

// React hooks for GraphQL queries
export const useGraphQLQuery = <T,>(
  client: GraphQLClient,
  query: string,
  variables?: Record<string, any>
) => {
  const [data, setData] = useState<T | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const execute = useCallback(async () => {
    setLoading(true);
    setError(null);

    try {
      const response = await client.query<T>(query, variables);

      if (response.errors) {
        setError(response.errors[0]?.message || 'Unknown error');
      } else if (response.data) {
        setData(response.data);
      }
    } catch (err) {
      setError(err instanceof Error ? err.message : 'Unknown error');
    } finally {
      setLoading(false);
    }
  }, [client, query, variables]);

  return { data, loading, error, execute };
};

export const useGraphQLMutation = <T,>(
  client: GraphQLClient,
  mutation: string
) => {
  const [data, setData] = useState<T | null>(null);
  const [loading, setLoading] = useState(false);
  const [error, setError] = useState<string | null>(null);

  const execute = useCallback(
    async (variables?: Record<string, any>) => {
      setLoading(true);
      setError(null);

      try {
        const response = await client.mutation<T>(mutation, variables);

        if (response.errors) {
          setError(response.errors[0]?.message || 'Unknown error');
        } else if (response.data) {
          setData(response.data);
        }
      } catch (err) {
        setError(err instanceof Error ? err.message : 'Unknown error');
      } finally {
        setLoading(false);
      }
    },
    [client, mutation]
  );

  return { data, loading, error, execute };
};
