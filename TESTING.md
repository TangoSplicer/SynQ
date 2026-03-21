# SynQ Testing Strategy & Coverage

## Overview

The SynQ project implements a comprehensive testing strategy across all components: compiler, backend, frontend, and IDE. This document outlines the testing framework, coverage goals, and best practices.

## Testing Architecture

### Test Pyramid

```
        /\
       /  \        E2E Tests (5%)
      /----\       Integration Tests (15%)
     /      \      Unit Tests (80%)
    /--------\
```

**Distribution:**
- **Unit Tests (80%)**: Fast, isolated, focused on individual components
- **Integration Tests (15%)**: Test component interactions and workflows
- **E2E Tests (5%)**: Full system testing from user perspective

## Component Testing

### Compiler Testing

**Location:** `compiler/tests/`

**Test Categories:**

1. **Unit Tests** (`compiler/tests/unit/`)
   - Lexer/tokenizer tests
   - Parser tests
   - Type checker tests
   - Code generator tests
   - Optimization tests

2. **Integration Tests** (`compiler/tests/integration/`)
   - Multi-file compilation
   - Cross-module dependencies
   - Linking and export
   - Plugin integration

3. **Security Tests** (`compiler/tests/security/`)
   - Memory safety validation
   - Type safety checks
   - Vulnerability scanning
   - Cryptographic tests

4. **Simulation Tests** (`compiler/tests/simulation/`)
   - Quantum circuit simulation
   - State vector validation
   - Measurement results
   - Algorithm verification

5. **AI Tests** (`compiler/tests/ai/`)
   - ML model inference
   - Neural network operations
   - Training validation
   - Optimization algorithms

**Running Compiler Tests:**

```bash
cd compiler/build
ctest --verbose                    # All tests
ctest -R "unit"                   # Unit tests only
ctest -R "security"               # Security tests only
ctest --output-on-failure         # Show failures
```

### Backend Testing

**Location:** `backend/app/tests/`

**Test Categories:**

1. **Unit Tests**
   - API endpoint tests
   - Service layer tests
   - Data model tests
   - Utility function tests

2. **Integration Tests**
   - Database integration
   - Redis caching
   - API workflows
   - Authentication flows

3. **API Contract Tests**
   - Request/response validation
   - Error handling
   - Status codes
   - Content types

4. **Performance Tests**
   - Response time validation
   - Throughput testing
   - Memory usage
   - Database query optimization

**Running Backend Tests:**

```bash
cd backend

# All tests
pytest tests/ -v

# With coverage
pytest tests/ --cov=src --cov-report=html

# Specific test file
pytest tests/test_phase9.py -v

# Specific test class
pytest tests/test_phase9.py::TestQuantumAPI -v
```

### Frontend Testing

**Location:** `frontend/src/__tests__/`

**Test Categories:**

1. **Unit Tests**
   - Component rendering
   - Props validation
   - State management
   - Event handlers

2. **Integration Tests**
   - Component interactions
   - API integration
   - Navigation flows
   - Form submission

3. **Visual Tests**
   - Snapshot tests
   - CSS validation
   - Responsive design
   - Accessibility

**Running Frontend Tests:**

```bash
cd frontend

# All tests
npm run test

# With coverage
npm run test -- --coverage

# Watch mode
npm run test -- --watch

# Specific test file
npm run test -- QuantumCircuit.test.tsx
```

## Test Coverage Goals

| Component | Target | Current | Status |
|-----------|--------|---------|--------|
| **Compiler** | 85% | 82% | ✅ On track |
| **Backend** | 80% | 78% | ✅ On track |
| **Frontend** | 75% | 72% | ✅ On track |
| **Overall** | 80% | 77% | ✅ On track |

## Continuous Integration

### GitHub Actions Workflows

**Compiler CI** (`.github/workflows/compiler-ci.yml`)
- Runs on: Push to main/develop, Pull requests
- Tests: Unit, integration, security
- Coverage: Reports to Codecov
- Artifacts: Test results, coverage reports

**Backend CI** (`.github/workflows/backend-ci.yml`)
- Runs on: Push to main/develop, Pull requests
- Tests: Unit, integration, API contract
- Coverage: Reports to Codecov
- Artifacts: Test results, coverage reports

### Pre-commit Hooks

Set up pre-commit hooks to run tests locally before committing:

```bash
# Install pre-commit
pip install pre-commit

# Set up hooks
pre-commit install

# Run hooks manually
pre-commit run --all-files
```

## Testing Best Practices

### 1. Test Naming

Use descriptive names that explain what is being tested:

```python
# ✅ Good
def test_quantum_circuit_adds_hadamard_gate_correctly():
    pass

# ❌ Bad
def test_circuit():
    pass
```

### 2. Arrange-Act-Assert Pattern

```python
def test_add_gate():
    # Arrange
    circuit = QuantumCircuit(2)
    
    # Act
    circuit.add_gate("h", [0])
    
    # Assert
    assert circuit.gate_count == 1
```

### 3. Test Isolation

Each test should be independent and not rely on other tests:

```python
# ✅ Good - Each test sets up its own state
def test_add_gate():
    circuit = QuantumCircuit(2)
    circuit.add_gate("h", [0])
    assert circuit.gate_count == 1

def test_measure_gate():
    circuit = QuantumCircuit(2)
    circuit.add_gate("measure", [0])
    assert circuit.gate_count == 1

# ❌ Bad - Test depends on previous test
circuit = QuantumCircuit(2)

def test_add_gate():
    circuit.add_gate("h", [0])
    assert circuit.gate_count == 1

def test_measure_gate():
    circuit.add_gate("measure", [0])
    assert circuit.gate_count == 2  # Depends on previous test
```

### 4. Test Data

Use fixtures for common test data:

```python
import pytest

@pytest.fixture
def quantum_circuit():
    return QuantumCircuit(2)

def test_add_gate(quantum_circuit):
    quantum_circuit.add_gate("h", [0])
    assert quantum_circuit.gate_count == 1
```

### 5. Mocking External Dependencies

Mock external services to isolate tests:

```python
from unittest.mock import patch, MagicMock

@patch('requests.get')
def test_api_call(mock_get):
    mock_get.return_value.json.return_value = {"result": "success"}
    
    result = call_external_api()
    
    assert result == {"result": "success"}
    mock_get.assert_called_once()
```

## Performance Testing

### Load Testing

Use k6 for load testing:

```bash
# Run load test
k6 run load-test.js

# With custom parameters
k6 run -e USERS=100 -e DURATION=5m load-test.js
```

### Profiling

Profile code to identify performance bottlenecks:

```bash
# Python profiling
python -m cProfile -s cumulative backend/main.py

# C++ profiling
perf record -g ./compiler/build/synq
perf report
```

## Test Maintenance

### Updating Tests

When code changes, update corresponding tests:

1. Identify affected tests
2. Update test expectations
3. Add new tests for new functionality
4. Ensure all tests pass

### Removing Tests

Only remove tests when:
- Feature is removed
- Test is duplicate
- Test is no longer relevant

Document why tests are removed in commit message.

## Coverage Reports

### Generating Coverage Reports

```bash
# Backend
cd backend
pytest --cov=src --cov-report=html
open htmlcov/index.html

# Frontend
cd frontend
npm run test -- --coverage
open coverage/lcov-report/index.html

# Compiler
cd compiler/build
cmake .. -DENABLE_COVERAGE=ON
make coverage
open coverage/index.html
```

### Interpreting Coverage

- **Line Coverage**: Percentage of lines executed
- **Branch Coverage**: Percentage of conditional branches tested
- **Function Coverage**: Percentage of functions called

**Coverage Goals:**
- Critical code: 90%+
- Important code: 80%+
- General code: 70%+

## Troubleshooting

### Common Issues

**Tests fail locally but pass in CI:**
- Check environment variables
- Verify database state
- Check file permissions
- Review test isolation

**Flaky tests:**
- Add proper waits/timeouts
- Mock time-dependent operations
- Use proper test fixtures
- Check for race conditions

**Slow tests:**
- Profile to identify bottlenecks
- Use mocks instead of real services
- Parallelize test execution
- Optimize database queries

## Resources

- [pytest Documentation](https://docs.pytest.org/)
- [Jest Documentation](https://jestjs.io/)
- [Google Test Documentation](https://github.com/google/googletest)
- [k6 Load Testing](https://k6.io/)

---

**Last Updated:** March 2026
**Maintained By:** SynQ Development Team
