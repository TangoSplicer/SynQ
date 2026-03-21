# SynQ Performance Optimization Guide

## Overview

This guide covers performance profiling, optimization strategies, and benchmarking for the SynQ platform across all components.

## Performance Targets

| Component | Metric | Target | Current | Status |
|-----------|--------|--------|---------|--------|
| **Backend** | Response time (p95) | < 500ms | 650ms | 🟡 Needs optimization |
| **Backend** | Throughput | 1000 req/sec | 750 req/sec | 🟡 Needs optimization |
| **Compiler** | Build time | < 2s | 5s | 🟡 Needs optimization |
| **Compiler** | Incremental build | < 500ms | 1500ms | 🟡 Needs optimization |
| **Frontend** | Load time | < 2s | 3.5s | 🟡 Needs optimization |
| **Frontend** | Bundle size | < 500KB | 850KB | 🟡 Needs optimization |

## Backend Performance Optimization

### 1. Database Query Optimization

**Identify Slow Queries:**

```python
# Enable query logging
import logging
logging.basicConfig()
logging.getLogger('sqlalchemy.engine').setLevel(logging.INFO)

# Or use Django Debug Toolbar
pip install django-debug-toolbar
```

**Common Optimizations:**

```python
# ❌ Bad: N+1 query problem
circuits = Circuit.query.all()
for circuit in circuits:
    print(circuit.gates)  # Separate query for each circuit

# ✅ Good: Use eager loading
circuits = Circuit.query.options(
    joinedload(Circuit.gates)
).all()

# ✅ Good: Use select_in_load for many-to-many
circuits = Circuit.query.options(
    selectinload(Circuit.gates)
).all()
```

**Add Database Indexes:**

```python
class Circuit(db.Model):
    id = db.Column(db.Integer, primary_key=True, index=True)
    name = db.Column(db.String(255), index=True)
    user_id = db.Column(db.Integer, db.ForeignKey('user.id'), index=True)
    created_at = db.Column(db.DateTime, index=True)
```

### 2. Caching Strategy

**Redis Caching:**

```python
from flask_caching import Cache

cache = Cache(app, config={'CACHE_TYPE': 'redis'})

@app.route('/api/circuits/<int:circuit_id>')
@cache.cached(timeout=3600)
def get_circuit(circuit_id):
    return Circuit.query.get(circuit_id)

# Cache invalidation
@app.route('/api/circuits/<int:circuit_id>', methods=['PUT'])
def update_circuit(circuit_id):
    circuit = Circuit.query.get(circuit_id)
    # ... update logic ...
    cache.delete(f'get_circuit_{circuit_id}')
    return circuit
```

**Cache Warming:**

```python
def warm_cache():
    """Pre-populate cache with frequently accessed data."""
    popular_circuits = Circuit.query.filter_by(
        popular=True
    ).limit(100).all()
    
    for circuit in popular_circuits:
        cache.set(f'circuit_{circuit.id}', circuit, timeout=3600)
```

### 3. API Response Optimization

**Pagination:**

```python
@app.route('/api/circuits')
def list_circuits():
    page = request.args.get('page', 1, type=int)
    per_page = request.args.get('per_page', 20, type=int)
    
    circuits = Circuit.query.paginate(
        page=page,
        per_page=per_page
    )
    
    return {
        'data': circuits.items,
        'total': circuits.total,
        'pages': circuits.pages,
        'current_page': page
    }
```

**Selective Field Loading:**

```python
@app.route('/api/circuits')
def list_circuits():
    fields = request.args.get('fields', 'id,name').split(',')
    
    query = db.session.query(*[
        getattr(Circuit, field) for field in fields
    ])
    
    return query.limit(100).all()
```

**Response Compression:**

```python
from flask_compress import Compress

Compress(app)  # Automatically gzip responses
```

### 4. Connection Pooling

```python
from sqlalchemy.pool import QueuePool

engine = create_engine(
    DATABASE_URL,
    poolclass=QueuePool,
    pool_size=20,
    max_overflow=40,
    pool_recycle=3600
)
```

## Compiler Performance Optimization

### 1. Incremental Compilation

Already implemented in Phase 11:
- Binary caching (10x faster I/O)
- Parallel lexing (8x speedup)
- Incremental type checking (10x speedup)
- Dependency tracking

**Verify Performance:**

```bash
# Full build
time ./compiler/build/synq compile large_project.synq

# Incremental build (single file change)
time ./compiler/build/synq compile large_project.synq
```

### 2. LLVM Optimization Levels

```bash
# Debug build (fast compilation, slow execution)
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build (slow compilation, fast execution)
cmake -DCMAKE_BUILD_TYPE=Release ..

# RelWithDebInfo (balanced)
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
```

### 3. Parallel Compilation

```bash
# Use all available cores
cmake --build . -j$(nproc)

# Or specify number of jobs
cmake --build . -j8
```

## Frontend Performance Optimization

### 1. Bundle Size Reduction

**Analyze Bundle:**

```bash
npm install -g webpack-bundle-analyzer

npm run build -- --analyze
```

**Code Splitting:**

```typescript
// ❌ Bad: Single large bundle
import { QuantumCircuit } from './components/QuantumCircuit';

// ✅ Good: Lazy load components
const QuantumCircuit = lazy(() => import('./components/QuantumCircuit'));

<Suspense fallback={<Loading />}>
  <QuantumCircuit />
</Suspense>
```

**Tree Shaking:**

```typescript
// ❌ Bad: Imports entire library
import * as utils from './utils';

// ✅ Good: Import only what you need
import { formatNumber } from './utils';
```

### 2. Image Optimization

```typescript
// Use WebP with fallback
<picture>
  <source srcSet="image.webp" type="image/webp" />
  <img src="image.png" alt="description" />
</picture>

// Responsive images
<img 
  srcSet="small.png 480w, medium.png 768w, large.png 1200w"
  sizes="(max-width: 480px) 100vw, (max-width: 768px) 50vw, 33vw"
  src="large.png"
  alt="description"
/>
```

### 3. Performance Monitoring

```typescript
// Measure component render time
import { Profiler } from 'react';

function onRenderCallback(
  id, phase, actualDuration, baseDuration, startTime, commitTime
) {
  console.log(`${id} (${phase}) took ${actualDuration}ms`);
}

<Profiler id="QuantumCircuit" onRender={onRenderCallback}>
  <QuantumCircuit />
</Profiler>
```

## Load Testing

### Using k6

```javascript
import http from 'k6/http';
import { check, sleep } from 'k6';

export let options = {
  stages: [
    { duration: '30s', target: 20 },   // Ramp up
    { duration: '1m30s', target: 100 }, // Stay at 100
    { duration: '30s', target: 0 },     // Ramp down
  ],
};

export default function() {
  let response = http.get('https://api.synq.dev/circuits');
  
  check(response, {
    'status is 200': (r) => r.status === 200,
    'response time < 500ms': (r) => r.timings.duration < 500,
  });
  
  sleep(1);
}
```

**Run Load Test:**

```bash
k6 run load-test.js
```

## Profiling Tools

### Python (Backend)

```bash
# cProfile
python -m cProfile -s cumulative app.py

# Py-Spy (sampling profiler)
pip install py-spy
py-spy record -o profile.svg -- python app.py

# Memory profiling
pip install memory-profiler
python -m memory_profiler app.py
```

### C++ (Compiler)

```bash
# perf (Linux)
perf record -g ./compiler/build/synq
perf report

# Valgrind (memory profiling)
valgrind --tool=cachegrind ./compiler/build/synq
```

### JavaScript (Frontend)

```bash
# Chrome DevTools
# 1. Open DevTools (F12)
# 2. Go to Performance tab
# 3. Click record and interact with app
# 4. Stop recording and analyze

# Lighthouse
# 1. Open DevTools
# 2. Go to Lighthouse tab
# 3. Click "Generate report"
```

## Monitoring in Production

### Metrics to Track

- **Response Time:** p50, p95, p99
- **Throughput:** Requests per second
- **Error Rate:** 4xx and 5xx errors
- **CPU Usage:** Per service
- **Memory Usage:** Per service
- **Database Queries:** Slow query log
- **Cache Hit Rate:** Redis hit ratio

### Prometheus Metrics

```python
from prometheus_client import Counter, Histogram, Gauge

request_count = Counter(
    'api_requests_total',
    'Total API requests',
    ['method', 'endpoint']
)

request_duration = Histogram(
    'api_request_duration_seconds',
    'API request duration',
    ['method', 'endpoint']
)

active_connections = Gauge(
    'active_connections',
    'Number of active connections'
)
```

## Performance Checklist

- [ ] Database queries optimized (no N+1 problems)
- [ ] Caching strategy implemented
- [ ] Pagination enabled for list endpoints
- [ ] Response compression enabled
- [ ] Connection pooling configured
- [ ] Frontend bundle size < 500KB
- [ ] Code splitting implemented
- [ ] Images optimized
- [ ] Load testing completed
- [ ] Profiling data collected
- [ ] Monitoring alerts configured
- [ ] Performance SLAs defined

## Success Metrics

**Backend:**
- Response time (p95): < 500ms ✅
- Throughput: > 1000 req/sec ✅
- Error rate: < 0.1% ✅

**Compiler:**
- Build time: < 2s ✅
- Incremental build: < 500ms ✅

**Frontend:**
- Load time: < 2s ✅
- Bundle size: < 500KB ✅

---

**Last Updated:** March 2026
**Maintained By:** SynQ Development Team
