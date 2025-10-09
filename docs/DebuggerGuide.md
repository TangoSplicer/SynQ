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
✅ File: synq/docs/DebuggerGuide.md

# SynQ Debugger Guide

## Overview

SynQ includes a built-in hybrid debugger that supports classical, quantum, and AI-augmented workflows. The debugger enables live introspection, stepwise circuit tracing, gate-level visualization, runtime memory state examination, and AI-enhanced diagnostics.

---

## 1. Starting the Debugger

You can run the debugger in three ways:

### CLI Debugger:
```bash
synq debug my_script.synq

Inline in REPL:

:debug

Embedded in Code:

debugger.start()


---

2. Breakpoints

Set breakpoints using:

breakpoint("label")

Or with the REPL/CLI command:

:break my_function

Breakpoints support both classical and quantum blocks.


---

3. Stepping Through Code

Command	Function

:step	Execute next line
:next	Step over function call
:continue	Resume until next breakpoint
:finish	Complete current function


You can also step through gates in a quantum circuit:

circuit.step_gates()


---

4. Visual Debugging

If visual mode is enabled:

:set visual true

The debugger will:

Highlight executed gates

Show qubit superpositions

Animate measurement collapses


Visual plugins:

svg_debug_viewer

qpu_live_sim



---

5. Memory Inspection

At any point during debugging, use:

:mem

Or inline:

debugger.inspect_memory()

Prints full variable state, including:

Classical variables

Qubit states

AI model outputs

Plugin state variables



---

6. Watch Expressions

Track the value of an expression over time:

:watch energyEstimate

List all watches:

:watches


---

7. Gate Tracing

Enable gate-by-gate tracing:

:set trace_gates true

The debugger will print:

[TRACE] H -> q[0]
[TRACE] CX -> q[0], q[1]

You may also export traces:

synq debug --trace --export trace.json


---

8. AI Assistance in Debugging

At any break, you can invoke AI to analyze state:

:ai Diagnose quantum register collapse error

Or explain state:

:ai Why is the output probability not uniform?


---

9. Plugin-Aware Debugging

Debugger integrates plugin runtime:

You can inspect plugin-injected variables

Hook into custom plugin trace events

Step through plugin-extended behavior


:plugin trace qaoa_optimizer


---

10. Debugging Tips

Use :trace with visual mode to identify decoherence

Use watch() to track optimizer convergence

If execution is non-deterministic, set:

:set seed 42



---

Known Limitations

Quantum circuit visualization requires enabled visual plugin

AI diagnostic requires a valid .synqrc with provider key

Not all embedded plugins are currently trace-compatible



---

Help

:debug help

or

synq debug --help

Documentation: synqhub.org/docs/debugger


---

© 2025 SynQ Project — Debug Everything, Even AI.

---


