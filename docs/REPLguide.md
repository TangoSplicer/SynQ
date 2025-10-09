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
✅ File: synq/docs/REPLGuide.md

# SynQ REPL Guide

The SynQ Read-Eval-Print Loop (REPL) provides an interactive environment that merges **classical, quantum, and AI-driven computation** in a unified shell. It’s designed to deliver real-time feedback, visualization, and automation during development.

---

## 🚀 Getting Started

To start the REPL:
```bash
synq repl

You’ll be greeted by:

synq>>

You can now run hybrid SynQ code interactively.


---

🔹 Key Features

1. Quantum-Classical Execution

Execute any hybrid code:

q = GHZ(3)
result = measure(q)
print(result)

2. AI Commands

Use :@ to issue AI-powered queries:

synq>> :@ Explain what a quantum circuit is

3. Inspector Commands

Inspect internal states, memory, and circuits:

synq>> :inspect qreg
synq>> :inspect mem
synq>> :inspect plugin

4. Plugin + Mutation Interface

Manage plugins live:

synq>> := load visual_debugger
synq>> := mutate current

5. Snapshots + Undo

synq>> :snapshot
synq>> x = 5
synq>> :undo

6. History & Trace

synq>> :history
synq>> :trace


---

🔍 Advanced REPL Commands

Command	Description

:@	Issue natural language prompt to AI
:=	Plugin load, mutate, or interact
:inspect	View memory, plugin state, quantum reg
:snapshot / :undo	Time-travel context
:plot	Generate live quantum/classical plots
:trace	Execution trace viewer
:editor	Toggle live inline editing
:config	Update REPL settings dynamically



---

📊 Visual + Notebook Support

REPL sessions can be visualized via:

notebooks/32_symbolic_command_repl.ipynb

notebooks/33_repl_ai_copilot.ipynb

notebooks/37_visual_shell_repl.ipynb

notebooks/47_sync_repl_demo.ipynb


Live REPL output can also be traced with:

synq>> :trace
synq>> :plot


---

🧠 AI Copilot in REPL

Integrates with SynQ Copilot agents

AI auto-completion and docstring feedback

Suggestions on quantum circuit designs



---

🔐 Secure + Isolated

All commands execute in a secure sandbox, with stateful rollback and plugin-bound security.


---

🛠 Developer Tips

Use :snapshot often when experimenting

Activate the debugger_overlay plugin for visual debugging

Combine inspect with plot for best clarity



---

📌 See Also

PluginGuide.md

DebuggerGuide.md

VisualTools.md



---

✅ Conclusion

The REPL is not just a shell — it's a fully integrated hybrid dev environment that can handle:

Live circuit synthesis

AI prompt interaction

Quantum-classical debugging

Plugin mutation and rollback


Explore. Iterate. Visualize. Live.



