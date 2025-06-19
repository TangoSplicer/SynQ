
✅ File: docs/LinterAI.md

# 🧠 SynQ Compiler Linter AI

---

## Overview

The SynQ Compiler Linter AI statically inspects your quantum IR and circuit topology, catching:

- 🌀 Circuit drift
- ⚠️ Semantic misuse
- ⏳ Depth or topology issues
- 🧠 Classical-quantum errors

---

## Lint Capabilities

| Code | Description | Severity |
|------|-------------|----------|
| `LINT_QUBIT_COLLISION` | Qubit used in overlapping gates | error |
| `LINT_CLASSICAL_COND` | Gate conditioned on classical bit | warning |
| `LINT_QUBIT_OVERUSE` | Qubit used too frequently | info |
| `LINT_DEPRECATED_SWAP` | Use of discouraged gate (e.g. swap) | info |
| `LINT_CIRCUIT_DEPTH` | Excessive logical depth on qubits | warning |

---

## Example CLI Usage

```bash
synq compile teleportation.synq --lint


---

Example Output

🔍 Linting quantum circuit...

⚠️ [LINT_CLASSICAL_COND] cx is conditionally applied based on classical register.
❌ [LINT_QUBIT_COLLISION] Qubit q2 used in overlapping gates at t=23
ℹ️ [LINT_QUBIT_OVERUSE] Qubit q0 used 14 times – consider reallocation.
✅ Compilation complete.


---

Notes

Requires pre-generated IR (e.g. parsed_ir.json)

Integrated via synq_compile.cpp with --lint flag

Will evolve to include layout and backend cost estimation



---

Future Features

Gate cost estimation

Topology-aware layout suggestions

Deep fusion optimization warnings



---

✅ Use the Linter AI to proactively improve quantum software correctness and efficiency!

---

✅ **Compiler Linter AI Complete**:
- Engine created
- CLI integrated
- Doc published



