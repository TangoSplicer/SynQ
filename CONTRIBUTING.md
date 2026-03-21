# Contributing to SynQ

Thank you for your interest in contributing to the SynQ project! This document provides comprehensive guidelines for contributing to all components of SynQ: the compiler, backend, frontend, and IDE.

## Table of Contents

1. [Code of Conduct](#code-of-conduct)
2. [Getting Started](#getting-started)
3. [Development Workflow](#development-workflow)
4. [Code Style Guidelines](#code-style-guidelines)
5. [Testing](#testing)
6. [Documentation](#documentation)
7. [Pull Request Process](#pull-request-process)
8. [Reporting Issues](#reporting-issues)

## Code of Conduct

We are committed to providing a welcoming and inspiring community for all. Please be respectful and constructive in all interactions.

## Getting Started

### Prerequisites

- Git
- C++ 17 compiler (GCC or Clang)
- CMake 3.15+
- Python 3.11+
- Node.js 22+
- Docker (optional)

### Setting Up Development Environment

```bash
# Clone the repository
git clone https://github.com/TangoSplicer/SynQ.git
cd SynQ

# Set up compiler
cd compiler
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Set up backend
cd ../backend
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
pip install -r requirements.txt

# Set up frontend
cd ../frontend
npm install
```

## Development Workflow

### 1. Create a Feature Branch

```bash
git checkout -b feature/your-feature-name
```

Use descriptive branch names:
- `feature/` for new features
- `fix/` for bug fixes
- `docs/` for documentation
- `refactor/` for code refactoring
- `test/` for tests

### 2. Make Your Changes

Follow the coding standards for your language (see Code Style Guidelines below).

### 3. Run Tests Locally

```bash
# Compiler tests
cd compiler/build
ctest --verbose

# Backend tests
cd backend
pytest tests/ -v --cov=src

# Frontend tests
cd frontend
npm run test
```

### 4. Commit Your Changes

Write clear, descriptive commit messages:

```
feat(compiler): Add quantum circuit optimization

- Implement circuit simplification algorithm
- Add unit tests for optimization
- Update documentation

Fixes #123
```

Follow the format:
- Type: feat, fix, docs, style, refactor, test, chore
- Scope: optional, e.g., (compiler), (backend), (frontend)
- Subject: clear, imperative mood
- Body: explain what and why, not how
- Footer: reference issues with "Fixes #123"

### 5. Push and Create Pull Request

```bash
git push origin feature/your-feature-name
```

Then create a pull request on GitHub with a clear description of your changes.

## Code Style Guidelines

### C++ (Compiler)

Follow Google C++ Style Guide with these conventions:

```cpp
// Use meaningful names
class QuantumCircuit {
public:
    void AddGate(const std::string& gate_name, const std::vector<int>& qubits);
    
private:
    std::vector<Gate> gates_;
};

// Use const and references appropriately
void ProcessCircuit(const QuantumCircuit& circuit);

// Add comments for complex logic
// Implement Grover's algorithm with oracle
void GroverSearch(QuantumCircuit& circuit, const Oracle& oracle);
```

**Formatting:**
- Use clang-format with Google style
- 4-space indentation
- 100-character line limit
- Use `_` suffix for member variables

### Python (Backend)

Follow PEP 8 with these conventions:

```python
"""Module for quantum circuit operations."""

from typing import List, Optional


class QuantumCircuit:
    """Represents a quantum circuit."""
    
    def add_gate(self, gate_name: str, qubits: List[int]) -> None:
        """Add a gate to the circuit.
        
        Args:
            gate_name: Name of the gate
            qubits: List of qubit indices
            
        Raises:
            ValueError: If qubit indices are invalid
        """
        pass
```

**Formatting:**
- Use Black for formatting
- Use isort for imports
- 4-space indentation
- Add type hints to all functions
- Write docstrings for all public functions

### JavaScript/TypeScript (Frontend)

Follow Airbnb style guide with these conventions:

```typescript
/**
 * Represents a quantum circuit visualization
 */
interface CircuitProps {
    gates: Gate[];
    qubits: number;
    onGateClick?: (gate: Gate) => void;
}

/**
 * Renders a quantum circuit diagram
 */
export const QuantumCircuit: React.FC<CircuitProps> = ({
    gates,
    qubits,
    onGateClick,
}) => {
    // Implementation
};
```

**Formatting:**
- Use Prettier for formatting
- 2-space indentation
- Use TypeScript for type safety
- Add JSDoc comments for functions

## Testing

### Writing Tests

**C++ (Compiler)**
```cpp
#include <gtest/gtest.h>
#include "quantum_circuit.h"

TEST(QuantumCircuitTest, AddGate) {
    QuantumCircuit circuit(2);
    circuit.AddGate("h", {0});
    EXPECT_EQ(circuit.GateCount(), 1);
}
```

**Python (Backend)**
```python
import pytest
from app.quantum import QuantumCircuit


def test_add_gate():
    """Test adding a gate to a circuit."""
    circuit = QuantumCircuit(2)
    circuit.add_gate("h", [0])
    assert circuit.gate_count == 1
```

**JavaScript (Frontend)**
```typescript
import { render, screen } from '@testing-library/react';
import { QuantumCircuit } from './QuantumCircuit';

test('renders quantum circuit', () => {
    render(<QuantumCircuit gates={[]} qubits={2} />);
    expect(screen.getByRole('canvas')).toBeInTheDocument();
});
```

### Running Tests

```bash
# All tests
npm run test  # Frontend
pytest       # Backend
ctest        # Compiler

# With coverage
pytest --cov=src --cov-report=html
npm run test -- --coverage
```

## Documentation

### Code Documentation

- Add docstrings/comments to all public functions
- Explain the "why", not just the "what"
- Include examples for complex functions
- Update README if adding new features

### Updating Documentation Files

- Update relevant `.md` files in `/docs`
- Keep documentation in sync with code
- Use clear, concise language
- Include examples and code snippets

## Pull Request Process

### Before Submitting

- Ensure all tests pass locally
- Run linters and formatters
- Update documentation if needed
- Add tests for new functionality
- Keep commits focused and logical

### PR Description Template

```markdown
## Description
Brief description of the changes

## Type of Change
- [ ] Bug fix
- [ ] New feature
- [ ] Breaking change
- [ ] Documentation update

## Testing
- [ ] Unit tests added
- [ ] Integration tests added
- [ ] Manual testing completed

## Checklist
- [ ] Code follows style guidelines
- [ ] Documentation updated
- [ ] Tests pass locally
- [ ] No new warnings generated
```

### Review Process

1. **Automated Checks**: CI/CD pipeline runs tests and linters
2. **Code Review**: Maintainers review the code
3. **Feedback**: Address any comments or suggestions
4. **Approval**: Maintainer approves the changes
5. **Merge**: PR is merged into main branch

## Reporting Issues

### Bug Reports

Include:
- Clear description of the bug
- Steps to reproduce
- Expected behavior
- Actual behavior
- Environment details (OS, version, etc.)
- Error messages or logs

### Feature Requests

Include:
- Clear description of the feature
- Motivation and use cases
- Proposed implementation (if applicable)
- Examples or mockups

## Security

- Never commit secrets or credentials
- Use environment variables for configuration
- Follow secure coding practices
- Report security issues privately
- Keep dependencies updated

## Questions?

- Check existing issues and discussions
- Read the documentation
- Ask in GitHub Discussions
- Email: support@synq.dev

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to SynQ! 🚀
