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
✅ File: synq/docs/Manual.md

# SynQ Manual

## Introduction

SynQ is a hybrid quantum-classical-AI programming language and compiler ecosystem. It enables seamless composition of quantum logic, classical control structures, and AI-powered operations in a single, elegant syntax.

---

## Language Highlights

- Simple Pythonic syntax with powerful classical constructs
- Native quantum gates, circuits, and measurement semantics
- AI models as first-class runtime objects
- Runtime and REPL hybrid execution engine
- Plugin-based extensibility with secure sandboxing
- Visual debugging and causal quantum tracing

---

## Core Concepts

| Concept   | Description |
|-----------|-------------|
| `Qubit`   | Core quantum data type, measured or entangled |
| `QRegister` | Quantum register abstraction |
| `model`   | Represents an AI model with config |
| `plugin`  | Dynamic module that extends the runtime |
| `run`     | Executes a model or function |
| `measure` | Collapses qubit state to classical bit |
| `circuit` | Container of ordered quantum operations |

---

## Hybrid Example

```synq
model GPT {
  name: "gpt-4"
  provider: "openai"
}

q = Qubit()
H(q)
result = measure(q)

explanation = run GPT("Explain the result: " + str(result))
print(explanation)


---

CLI Tools

SynQ CLI provides developer-first command line tools:

🧰 Core Commands

synq create        # Scaffold new plugin or project
synq build         # Compile source
synq run           # Run a SynQ file
synq test          # Execute tests
synq plugin        # Manage plugins
synq deploy        # Cloud or local job execution
synq debug         # Launch debugger
synq ai            # Query or configure AI models
synq config        # Set environment vars in .synqrc

🚀 Advanced Tools

synq mutate        # AI-guided plugin evolution
synq fuzz          # Prompt fuzzing for model testing
synq qtrace        # Quantum causal trace visualizer
synq inspect       # Analyze plugins/projects
synq skillmap      # Feature heatmap + usage insights
synq chain         # Distributed pipeline/job runner
synq sign          # QRYPTA-TGIL signature verifier

🛠️ System Commands

synq version       # Show compiler version
synq help          # CLI help menu
synq doctor        # Environment diagnostics
synq init          # Minimal SynQ starter project
synq man <cmd>     # Manual entry for any tool


---

Memory & Debug Tools

:mem shows hybrid state

:trace overlays quantum trace

:ai asks model to explain current state

:plugin load visual_debugger enables SVG output



---

Learn More

Developer Guide → docs/DeveloperGuide.md

CLI Reference → docs/CLI_Toolchain.md

Mutation Engine → docs/MutationEngine.md

Fuzzing → docs/FuzzingAndSecurity.md



---

© 2025 SynQ Project — Hybrid Coding Reinvented.
   
   ⚖️ Open Source License
   This project is released under the MIT License - free for everyone to use, modify, and distribute.

---

✅

