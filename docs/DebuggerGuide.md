// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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


