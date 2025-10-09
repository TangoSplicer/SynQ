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

# 🌐 SynQ Language — Quantum-Classical-AI Programming Platform

> **As simple as Python, as powerful as Java.**
>  
> SynQ is a next-gen hybrid programming language enabling seamless integration of classical computation, quantum circuits, and AI models — all within one maximally enhanced, unified platform.

---

## 🚀 Key Highlights

- 🧠 **AI Copilot Integration**
- ⚛️ **Native Quantum Circuit Syntax**
- 💻 **Efficient Classical Runtime & Type System**
- 🔌 **Live Plugin Mutation + Registry**
- 🧪 **Interactive REPL with Time Travel**
- 📦 **Built-in Notebooks, Visual Tools, Debugger**
- 🛡️ **Quantum-Safe Encryption with QRYPTA-TGIL**

---

## 📁 Project Structure

synq/ ├─ src/              # Core compiler, runtime, REPL, plugin, and encryption logic │   ├─ repl/         # REPL core + inspect + interactive commands │   ├─ cli/          # CLI toolchain (ai, mutate, fuzz, sign, debug, etc.) │   ├─ core/         # Language internals, tokenization, type system │   ├─ plugins/      # Plugin loader, mutation, registry management │   └─ quantum/      # Quantum simulation and logic (QPELA, QEBET, QGraphISO) ├─ stdlib/           # Built-in modules for quantum, AI, classical operations ├─ tests/            # Full unit/integration test suite ├─ notebooks/        # AI demos, circuit visualization, plugin mutation ├─ docs/             # Reference guides and manuals ├─ build/            # Compiled output, binaries, signed bundles ├─ CMakeLists.txt    # Cross-platform build config └─ README.md         # Project entry point (this file)

---

## 🧠 Language Capabilities

```synq
model GPT {
  name: "gpt-4"
  provider: "openai"
}

circuit Bell {
  q0 = |0>
  h(q0)
  cnot(q0, q1)
}

print(run GPT("Explain the Bell circuit"))
```

Supports:

Quantum entanglement

Tensor algebra

AI prompt pipelines

Classical interop

Plugin-controlled mutation



---

🧪 REPL Features

Start with:

synq repl

Then:

Syntax	Description

:@	Ask AI questions
:=	Load/mutate plugin
:inspect	Debug memory, quantum registers
:snapshot	Save & rollback state
:plot	Visual entanglement/state graphs
:undo, :history, :man	Developer-quality controls


Includes AI trace explanations, REPL visualization, plugin debugger overlay, and interactive mutation.


---

🔧 CLI Toolchain

Invoke with synq [command]. Available tools:

Command	Description

ai	Run AI prompts and circuit-to-doc generation
mutate	AI-guided plugin mutation system
fuzz	Prompt fuzzing for plugin/circuit safety
sign	Quantum-safe signing of plugins
inspect	Full codebase + metadata introspection
qtrace	Gate visualizer + causal execution tree
skillmap	Developer interaction + learning visualizer
chain	Distributed job + pipeline execution
debug	Interactive debugger and quantum backtrace
config	Edit CLI/runtime configuration profiles






---

🔌 Plugin Ecosystem

Plugins live inside:

src/plugins/

Support:

Dynamic load/unload

Mutation via synq mutate

AI-assisted creation

Signing via synq sign


Registered plugins can be published to SynQHub. //# TO BE IMPLEMENTED 


---

🧪 Built-In Quantum Algorithms

File	Description

qgraphiso.synq	Quantum isomorphism + encryption base
qpela.synq	Phase estimation lattice
qebet.synq	Entanglement-based error tracking
qtms.py	Tensorized message signing (AEBS)





---

📦 Building & Running

mkdir build && cd build
cmake ..
make -j8
./synq repl

For signed plugin output:

synq sign build/plugin.so




---

📚 Documentation

All manuals live in:

docs/

Start with:

Manual.md

REPLGuide.md

PluginGuide.md

CompilerInternals.md

VisualTools.md




---

⚖️ Open Source License

This project is released under the MIT License.
See LICENSE.txt for full terms.

The SynQ language is now free and open source for everyone to use, modify, and distribute.


---

✅ Conclusion

Welcome to SynQ — the future of hybrid quantum-classical-AI computation, now open source for the community.
