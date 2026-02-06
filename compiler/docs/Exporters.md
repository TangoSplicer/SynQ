
✅ File: docs/Exporters.md

📘 Guide to Exporting SynQ Programs to VM, WASM, LLVM


---

# 🔄 SynQ Exporters

SynQ supports exporting compiled programs to portable formats:

- `.synqvm` — SynQ Virtual Machine bytecode
- `.wasm` — WebAssembly for browser-based runtimes
- `.ll` — LLVM IR for low-level target integration

---

## Usage

```bash
synq export teleportation.synq --target synqvm
synq export teleportation.synq --target wasm
synq export teleportation.synq --target llvm


---

Output Files

Target	Extension	Description

synqvm	.synqvm	SynQVM bytecode
wasm	.wasm	WebAssembly module (text format)
llvm	.ll	LLVM IR (for toolchain integration)



---

CLI Wrapper

The command is powered by:

synq_export.cpp → calls exporter.py


---

Program Requirements

Source files must be valid .synq programs already transpiled or compatible with the IR loader.


---

Examples

Sample file: examples/export/sample.synq

Run:

synq export examples/export/sample.synq --target llvm


---

Next Steps

Future releases will allow:

Compiling full .synq into binary .synqvm modules

Remote execution on SynQVM Runtime Cluster


---

### ✅ File: `examples/export/sample.synq`  
🔬 A minimal test program for export pipeline

---

```text
// SynQ Sample Export Program
qbit a, b;
H a;
CNOT a, b;
measure a;
measure b;


---