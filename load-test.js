import http from 'k6/http';
import ws from 'k6/ws';
import { check, group, sleep } from 'k6';
import { Rate, Trend, Counter, Gauge } from 'k6/metrics';

// Custom metrics
const errorRate = new Rate('errors');
const apiDuration = new Trend('api_duration');
const wsConnections = new Gauge('ws_connections');
const requestCount = new Counter('requests');

export const options = {
  stages: [
    // Ramp up to 100 users over 2 minutes
    { duration: '2m', target: 100 },
    // Stay at 100 users for 5 minutes
    { duration: '5m', target: 100 },
    // Ramp up to 200 users over 2 minutes
    { duration: '2m', target: 200 },
    // Stay at 200 users for 5 minutes
    { duration: '5m', target: 200 },
    // Ramp down to 0 users over 2 minutes
    { duration: '2m', target: 0 },
  ],
  thresholds: {
    'http_req_duration': ['p(95)<500', 'p(99)<1000'],
    'http_req_failed': ['rate<0.1'],
    'errors': ['rate<0.05'],
  },
};

const BASE_URL = __ENV.BASE_URL || 'http://localhost:8000';
const WS_URL = __ENV.WS_URL || 'ws://localhost:8000';

export default function () {
  // Test API endpoints
  group('API Tests', function () {
    // Health check
    group('Health Check', function () {
      let res = http.get(`${BASE_URL}/health`);
      check(res, {
        'health status is 200': (r) => r.status === 200,
        'health response time < 100ms': (r) => r.timings.duration < 100,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 200);
    });

    // Circuit endpoints
    group('Circuit Operations', function () {
      // Get circuits
      let res = http.get(`${BASE_URL}/api/v1/circuits`);
      check(res, {
        'get circuits status is 200': (r) => r.status === 200,
        'get circuits response time < 500ms': (r) => r.timings.duration < 500,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 200);

      // Create circuit
      let payload = JSON.stringify({
        name: `test-circuit-${Date.now()}`,
        description: 'Load test circuit',
        qubits: 5,
      });

      let params = {
        headers: {
          'Content-Type': 'application/json',
        },
      };

      res = http.post(`${BASE_URL}/api/v1/circuits`, payload, params);
      check(res, {
        'create circuit status is 201': (r) => r.status === 201,
        'create circuit response time < 1000ms': (r) => r.timings.duration < 1000,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 201);
    });

    // Compilation endpoints
    group('Compilation Operations', function () {
      let payload = JSON.stringify({
        circuit_id: 'test-circuit',
        target_backend: 'ibm_quantum',
        optimization_level: 3,
      });

      let params = {
        headers: {
          'Content-Type': 'application/json',
        },
      };

      let res = http.post(`${BASE_URL}/api/v1/compile`, payload, params);
      check(res, {
        'compile status is 200 or 202': (r) => r.status === 200 || r.status === 202,
        'compile response time < 2000ms': (r) => r.timings.duration < 2000,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 200 && res.status !== 202);
    });

    // ML Predictions
    group('ML Predictions', function () {
      let payload = JSON.stringify({
        circuit_id: 'test-circuit',
        prediction_type: 'resource_estimation',
      });

      let params = {
        headers: {
          'Content-Type': 'application/json',
        },
      };

      let res = http.post(`${BASE_URL}/api/v1/ml/predict`, payload, params);
      check(res, {
        'ml predict status is 200': (r) => r.status === 200,
        'ml predict response time < 1000ms': (r) => r.timings.duration < 1000,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 200);
    });

    // GraphQL queries
    group('GraphQL Queries', function () {
      let payload = JSON.stringify({
        query: `
          query {
            circuits(limit: 10) {
              id
              name
              qubits
            }
          }
        `,
      });

      let params = {
        headers: {
          'Content-Type': 'application/json',
        },
      };

      let res = http.post(`${BASE_URL}/graphql`, payload, params);
      check(res, {
        'graphql status is 200': (r) => r.status === 200,
        'graphql response time < 500ms': (r) => r.timings.duration < 500,
      });
      requestCount.add(1);
      apiDuration.add(res.timings.duration);
      errorRate.add(res.status !== 200);
    });
  });

  // Test WebSocket connections
  group('WebSocket Tests', function () {
    let url = `${WS_URL}/ws`;
    let params = {
      tags: { name: 'WebSocketTest' },
    };

    let res = ws.connect(url, params, function (socket) {
      socket.on('open', function () {
        wsConnections.add(1);
        console.log('WebSocket connected');

        // Send authentication
        socket.send(JSON.stringify({
          type: 'auth',
          token: 'test-token',
        }));

        // Send subscription
        socket.send(JSON.stringify({
          type: 'subscribe',
          channel: 'circuit-updates',
        }));
      });

      socket.on('message', function (data) {
        console.log('Received message:', data);
      });

      socket.on('close', function () {
        wsConnections.add(-1);
        console.log('WebSocket closed');
      });

      socket.on('error', function (e) {
        console.log('WebSocket error:', e);
        errorRate.add(1);
      });

      // Keep connection open for 30 seconds
      socket.setTimeout(function () {
        socket.close();
      }, 30000);
    });

    check(res, {
      'websocket connection successful': (r) => r.status === 101,
    });
  });

  sleep(1);
}

export function handleSummary(data) {
  return {
    'stdout': textSummary(data, { indent: ' ', enableColors: true }),
    'summary.json': JSON.stringify(data),
  };
}

function textSummary(data, options) {
  let summary = '\n=== Load Test Summary ===\n';
  
  if (data.metrics) {
    summary += '\nMetrics:\n';
    for (let [name, metric] of Object.entries(data.metrics)) {
      summary += `  ${name}: ${JSON.stringify(metric.values)}\n`;
    }
  }

  if (data.state.isStdOutTTY) {
    summary += '\n✓ Test completed successfully\n';
  }

  return summary;
}
