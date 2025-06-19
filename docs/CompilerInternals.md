// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

✅ File: synq/docs/CompilerInternals.md

# SynQ Compiler Internals

## Overview

The SynQ compiler is designed as a hybrid, pluggable, and introspectable quantum-classical-AI compilation engine. It supports high-level SynQ source code, transforming it into quantum circuits, classical bytecode, plugin-enhanced operations, and AI-augmented pipelines.

---

## 1. Compilation Pipeline Overview

Source Code (.synq) ↓ Lexer & Parser (ANTLR) ↓ AST Construction ↓ Type Checking & Inference ↓ Pass Manager (Classical + Quantum + AI) ↓ IR Generation (Hybrid) ↓ Backends (Circuit, Binary, LLVM, Plugins)

---

## 2. Lexer & Parser

- Built using **ANTLR4**, with an extended grammar supporting:
  - Quantum constructs (`Qubit`, `Measure`, etc.)
  - AI calls (`model`, `run`, etc.)
  - Plugin expressions
- Output: `ASTNode` graph

---

## 3. AST (Abstract Syntax Tree)

Central node types:
- `ASTFunction`, `ASTCircuit`, `ASTModel`, `ASTPluginCall`, `ASTQubit`, etc.

Each node tracks:
- Type signature
- Parent scope
- Inferred behaviors
- Line number and context

---

## 4. Type System

Hybrid static+dynamic system:
- Classical types: `int`, `float`, `str`, `bool`, `list`, `dict`, etc.
- Quantum types: `Qubit`, `QRegister`, `QTensor`
- AI types: `Prompt`, `Response`, `ModelOutput`

Supports:
- Type inference
- Operator overloading
- Hybrid type conversion

---

## 5. Intermediate Representation (IR)

SynQ uses a **hybrid IR**:
- **Classical IR**: Similar to bytecode
- **Quantum IR**: Gate-labeled, DAG-formatted
- **AI IR**: Prompt-tree sequences and cost annotations

This hybrid IR feeds into multiple backends.

---

## 6. Optimization Passes

Managed via a central `PassManager`:
- Constant folding
- Gate merging
- Redundant measurement pruning
- AI path scoring
- Dead code + plugin branch elimination

---

## 7. Backends

| Backend       | Output                          |
|---------------|---------------------------------|
| `qasm_backend`| OpenQASM 3.0                    |
| `llvm_backend`| LLVM IR for classical code      |
| `json_backend`| Hybrid job JSON (for SynQHub)   |
| `ai_backend`  | Token trace + cost estimator    |
| `plugin_backend`| Routed plugin calls            |

Backends can be mixed per function block.

---

## 8. Plugin Engine Integration

Plugins can inject:
- New nodes into AST
- Intermediate passes into IR
- Finalizers into backends

Safeguarded by:
- `plugin_policy.cpp`
- `sandbox_violation.h`
- `refactor_planner.h` (for plugin-guided mutation)

---

## 9. AI in the Compiler

The AI layer supports:
- Prompt generation from code context
- Embedding similarity lookups
- Code transformation proposals
- Language-model-driven optimizations

AI tools live in `compiler/ai/ai_toolkit.{cpp,h}`

---

## 10. Visual Compiler Trace

Enabled via:
```bash
synq --trace

Displays:

AST walk diagram

Pass timing

IR diffs

Final output tree


Optionally renders to:

.svg, .json, .html



---

11. Multi-Pass Mode

Enable deep recursion through:

synq --multi-pass

Used for:

Layered plugin injection

Classical control inside quantum loops

Quantum-AI model interaction inside circuits



---

12. Future Compiler Features

WASM output for embedded interpreters

Live-edit + hot-reload of compiled circuits

Type-theory guided optimizers

Self-evolving compiler using plugin scoring and AI mutation



---

Resources

Compiler source: src/compiler/

Plugins: src/compiler/sandbox/, src/compiler/optimize/

IR schemas: docs/CodegenTargets.md, docs/Quantum_Features.md



---

© 2025 SynQ Project — Compiler of the Future: Hybrid, Extensible, Aware.

---

