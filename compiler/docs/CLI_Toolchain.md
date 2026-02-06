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
✅ New File: synq/docs/CLI_Toolchain.md

# SynQ CLI Toolchain Reference

## Overview

The SynQ CLI is a powerful suite of tools designed to manage every part of the hybrid development lifecycle — from scaffolding, testing, and tracing to mutation, AI fuzzing, deployment, and plugin security.

This guide outlines **all CLI commands**, grouped by category, with usage examples.

---

## 🧰 Core Commands

### `synq create`
Scaffold a plugin or project.
```bash
synq create plugin my_visualizer
synq create project entanglement_app

synq build

Compile your project.

synq build

synq run

Run any SynQ .synq file.

synq run src/main.synq

synq test

Execute unit tests.

synq test

synq plugin

Plugin management.

synq plugin build my_plugin
synq plugin validate my_plugin
synq plugin publish my_plugin

synq debug

Launch debugger view.

synq debug src/main.synq


---

🧠 AI + Inspection Tools

synq ai

Send a prompt to an AI model.

synq ai "What is quantum decoherence?"

synq profile

Show execution time and memory.

synq profile src/qpe.synq

synq trace

Print step-by-step trace.

synq trace examples/shor.synq

synq config

Set or read from .synqrc.

synq config ai_key sk-xyz...


---

🔬 Advanced Developer Tools

synq mutate

Evolve a plugin into improved variants.

synq mutate plugins/qopt_pass

synq fuzz

Test AI plugin safety via adversarial input.

synq fuzz llama-3

synq qtrace

Visualize quantum gate/entanglement causality.

synq qtrace src/circuit.synq

synq inspect

Analyze a plugin or project.

synq inspect .

synq skillmap

Generate a feature usage heatmap.

synq skillmap

synq chain

Run chained SynQ programs.

synq chain qaoa.synq optimize.synq verify.synq

synq sign

Digitally sign outputs/plugins (QRYPTA-TGIL).

synq sign build/output.json


---

🧱 System Commands

synq doctor

Check system readiness.

synq doctor

synq version

Show version info.

synq version

synq init

Scaffold a minimal project layout.

synq init

synq help

Show command listing.

synq help

synq man

Command-specific documentation.

synq man mutate


---

Tips

Use chain to parallelize and scale workflows

Use sign for secure, reproducible builds

Use fuzz before publishing any AI-enhanced plugin

Use mutate to evolve your toolchain over time



---

© 2025 SynQ Project — The Most Advanced CLI for the Future of Code.

.

