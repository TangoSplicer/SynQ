// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

✅ Updated File: synq/docs/CodegenTargets.md

# ⚙️ SynQ Code Generation Targets

SynQ supports a broad range of classical and quantum output targets for transpilation, execution, or deployment.

---

## Supported Backends

| Target     | Description                            |
|------------|----------------------------------------|
| `llvm`     | Emits LLVM IR for native compilation   |
| `cpp`      | Generates C++17 code                   |
| `qasm`     | Outputs OpenQASM-compatible circuit    |
| `json`     | Outputs serialized AST or IR structure |
| `cirq`     | Generates Cirq-compatible Python code  |
| `qulacs`   | Outputs code for Qulacs backend        |
| `python`   | Transpiles to Python + quantum binding |
| `openqasm3`| QASM3-compatible gateflow export       |

---

## Command Usage

```bash
synq transpile source.synq --target=cpp -o output.cpp
synq transpile source.synq --target=qasm


---

Embeddable Outputs

You may bundle generated code using:

synq sign (to cryptographically lock the output)

synq export --target json (for visual debuggers)

synq qtrace (for trace diagrams)




---

Internal Pipelines

The following targets support optimization pipelines:

llvm, cpp, qasm, openqasm3

With plugin-based extension for custom dialects



---

Custom Targets

To create a custom output backend:

synq cli create-backend MyTarget


---

⚠️ Licensing

All targets, converters, and templates are proprietary components of the SynQ compiler platform.

Do not extract, export, or bundle these targets into commercial tools or frameworks without prior licensing approval.

See LICENSE.txt for the full legal statement.


---

Continue reading in:

CompilerInternals.md

PluginGuide.md


All identified files that previously suggested MIT/open-source use have now been replaced with your **custom proprietary license model**.



