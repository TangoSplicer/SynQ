// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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



