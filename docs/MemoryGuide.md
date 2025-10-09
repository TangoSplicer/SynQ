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
✅ File: synq/docs/MemoryGuide.md

# SynQ Memory Guide

## Overview

SynQ’s memory system unifies classical variables, quantum registers, plugin states, and AI outputs into a single inspectable memory graph. This guide covers memory management, introspection tools, serialization formats, performance profiling, and optimization strategies.

---

## 1. Memory Architecture

SynQ’s memory model is **hybrid-layered**:

- **Classical Layer**: integers, floats, strings, structures
- **Quantum Layer**: Qubit, QRegister, QTensor
- **AI Layer**: Prompt traces, model outputs, embeddings
- **Plugin Layer**: Plugin-specific runtime memory

Each memory object is registered and trackable by its symbolic name, lifecycle scope, and type signature.

---

## 2. Inspecting Memory

### REPL Command:
```synq
:mem

Inline in Code:

memory.inspect()

Returns:

{
  "classical": {"x": 42, "alpha": 0.9},
  "quantum": {"q0": "|0>", "q1": "|+>"},
  "ai": {"last_response": "Entanglement..."},
  "plugin": {"visual_mode": true}
}


---

3. Saving and Loading Memory

Save:

:save session.mem

Load:

:load session.mem

Uses .mem file format (JSON-based hybrid serialization).


---

4. Memory Snapshots in Code

You can snapshot memory at any stage:

state = memory.snapshot()

Restore later:

memory.restore(state)


---

5. Quantum State Export

You can export full register states:

qreg = QRegister(3)
export_state(qreg, "state_vector.csv")

Options:

Bloch vector

Density matrix

Statevector or circuit trace



---

6. AI Memory

All AI interactions are logged:

Prompt history

Completion trace

Model confidence scores (if available)


To retrieve:

ai.memory("last_prompt")
ai.memory("token_usage")


---

7. Plugin Memory Access

Plugins register memory via:

register_plugin_memory("visual_mode", true);

Access via:

plugin.mem("visual_mode")


---

8. Memory Visualization

With visual plugins enabled:

:set visual true
:mem

Displays:

Memory heatmaps

Qubit entanglement diagrams

Classical/Quantum correlation graphs



---

9. Memory Profiling

Use memory.profile() to see:

Allocation sizes

Lifetime

Type heatmaps

Plugin usage share


:profile memory

Can be used to find leaks or bloated variables.


---

10. Best Practices

Use memory snapshots before optimization passes

Clean up plugin state after long chains

Avoid global classical reuse inside quantum control blocks

Serialize AI outputs selectively to prevent overload



---

Known Limitations

.mem format is not optimized for large QTensor graphs (use .qdump for those)

Plugin memory access is read-only unless explicitly declared mutable



---

Support

Use:

:help memory

Or visit: synqhub.org/docs/memory


---

© 2025 SynQ Project — Memory is Modular, Traceable, and Reversible.

---



