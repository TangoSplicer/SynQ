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
# SynQ Language Manual 📘 (Extended Edition)

Welcome to **SynQ** — the DSL where quantum circuits, AI agents, and classical logic cohabitate peacefully in structured parentheses.

This expanded edition dives deeper into SynQ's roots, theoretical basis, magical tools, and design quirks. If the regular manual was a well-mixed Bell state, this is your QFT-decomposed version with annotations.

---

## 🧬 Origin of SynQ

SynQ began as an answer to a serious question:

> "What if you could write quantum and AI code side-by-side without crying?"

And thus, SynQ was born:  
- "Syn" from **synthesis**, symbolizing hybrid AI + Quantum
- "Q" from **quantum** (naturally)

Its spiritual ancestors include Python, Haskell, and the frustrated souls who’ve written OpenQASM parsers by hand.

---

## 🔬 Theoretical Algorithms

### QEBET – Quantum Enhanced Basis Embedding Tree

- An original encoding algorithm using **basis-partitioned quantum trees**
- Designed for fast traversal and superposition-aware graph embedding
- Used internally by SynQ’s symbolic circuit tools and QML routines

### QPE-LA – Quantum Phase Estimation with Latent Alignment

- Extends standard QPE with a **latent feedback loop**
- Captures phase estimates, stores them as latent vectors, aligns with ML-predicted distributions
- Useful in signal calibration, molecular simulation

### GraphISO (QGraphISO)

- A native quantum-friendly **graph isomorphism analyzer**
- Operates over adjacency matrix encodings with phase-simulated walks
- Tensorized support for encrypted states (via TGIL from QRYPTA integration)

---

## 🏛️ Compiler Philosophy

The SynQ compiler is:
- Written in C++
- Fronted by a symbolic DSL
- Built on love, macros, and slightly obsessive AST normalization

### Architecture Highlights:
- `parser.h` + `ast.h` = Your best friends
- `qsimla.cpp` = Simulation with snapshots and vector math
- `plugin_store.cpp` = The cool kid registry for runtime tools
- `model_provider.h` = How SynQ talks to LLMs like they're coworkers

---

## 🤯 Design Principles

- **Clarity over cleverness**: No magical metaprogramming unless it buys you back tenfold
- **Quantum-native + AI-native**: Both must be first-class
- **Sandboxed extensibility**: Plugins are sandboxed via interfaces
- **Latent-awareness**: Simulators and models speak "latent vector" fluently

---

## 🧠 Agentic Architecture

Every AI agent in SynQ:
- Has a memory store (structured, retrievable)
- Can access models, rerankers, and tools
- Evolves contextually via `call_model()` and `remember()`

---

## 💫 Profiling + Visualizing Quantum Programs

Use:
```synq
@profiler_start("block")
...
@profiler_end("block")
```

Or visualize:
```synq
@visual_start
H(0)
@visual_end("viz", title="Circuit Title")
```

---

## 🔮 Future Roadmap (Post 1.0)

- Support for **real-time quantum hardware backends**
- Integration with **cloud AI orchestration APIs**
- Multi-agent cooperative reasoning templates
- Encrypted QPU streams via TGIL lattice isolation

---

## 📚 Final Thoughts

SynQ is built for those who:
- Like their circuits expressive
- Want models that talk *to* code, not just inside it
- Need agents that remember *why* they made that decision

Thanks for reading. Now go forth, and entangle responsibly.

— The SynQ Compiler & DSL Team namely one guy J.M.A.M-O (and some very smart electrons)
