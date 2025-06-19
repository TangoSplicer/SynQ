// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.


---

📄 File: synq/docs/ExporterGuide.md

# SynQ Exporter Guide

> “Export everything. Circuits, ASTs, timelines, AI prompts, and your wildest ideas.”

---

## 🎯 Purpose

SynQ exporters allow the transformation of internal program structures — quantum circuits, ASTs, AI prompt chains, runtime data — into formats usable outside the language.

You can:
- Export quantum circuits to QASM or QIR
- Export ASTs to JSON, binary, or GraphViz
- Export runtime data as timelines, coverage maps
- Export plugin-defined custom formats

---

## 🗃️ Built-in Export Formats

| Format   | Type        | Target                           |
|----------|-------------|----------------------------------|
| `qasm`   | Quantum     | OpenQASM (IBMQ, simulators)      |
| `qir`    | Quantum     | Quantum IR (LLVM-style)          |
| `json`   | Universal   | AST, debug symbols, structures   |
| `bin`    | Universal   | Binary IR                        |
| `flow`   | Runtime     | Flow graph (Graphviz SVG)        |
| `html`   | Debug       | Timeline, trace, memory map      |

---

## 🚀 CLI Usage

```bash
synq export teleportation.synq --format qasm
synq export foo.synq --format flow

With a plugin:

synq export core.synq --plugin my_custom_exporter

You can also combine formats:

synq export ai_demo.synq --format json --format html


---

🔌 Language-based Triggers

Use @export or plugin tags:

@export "circuits/entangle.qasm"
@plugin "qgraphiso.export"

This causes the compiler to:

1. Tag the AST


2. Route through exporter registry


3. Dump to output directory or target




---

🛠 Create a New Exporter

1. File Location

Create a .cpp file in:

src/compiler/exporters/my_exporter.cpp

And its header in:

src/compiler/exporters/my_exporter.h

2. Register Exporter

In plugin_registry.cpp:

register_exporter("my_format", &export_my_format);

3. Function Signature

void export_my_format(const ASTNode& root, std::ostream& out);

Or for circuits:

void export_my_format(const QuantumCircuit& qc, std::ostream& out);

Or for runtime trace:

void export_runtime_trace(const RuntimeProfiler& rp, const std::string& output_path);


---

🧠 Exporter Plugin Types

Plugin Interface	Triggered On	Output

exporter_ast	AST Build	JSON, Bin, DOT
exporter_circuit	Circuit Ready	QASM, QIR
exporter_runtime	Post-execution	Coverage, Trace
exporter_plugin	Manual Trigger	Anything



---

🧬 Full Example

// src/compiler/exporters/export_qgraph.cpp
void export_qgraph(const QuantumCircuit& qc, std::ostream& out) {
    out << "QGRAPH::BEGIN\n";
    for (auto& gate : qc.gates()) {
        out << gate.name << " ";
        for (auto q : gate.targets)
            out << "q" << q << " ";
        out << "\n";
    }
    out << "QGRAPH::END\n";
}


---

📦 Export Metadata

Exported files include metadata headers:

{
  "exported_by": "SynQ",
  "version": "1.0.0",
  "format": "qasm",
  "timestamp": "2025-06-14T14:22:51Z",
  "source_file": "teleportation.synq"
}


---

🧪 Testing Exporters

Inside test files:

@plugin "export_qgraph"
@export "out/graph.qgraph"

Or via test_runner:

run_test("exporter_qgraph", "tests/export/qgraph_test.synq");


---

🌈 Visual Exporters

Export to SVG / HTML:

render_to_svg(ast, "trace.svg");
export_flowtrace_html(runtime, "timeline.html");


---

🔗 Integrated Export Stack

Exporters are integrated into:

Compiler (transpile + tag phase)

Runtime (after run())

Debugger (after failure)

Plugins (hooked on signals)



---

🛑 Common Errors

Error	Fix

Missing file output	Ensure path is writable
Invalid AST node	Your plugin assumes wrong node type
Not registered	Add to register_exporters()



---

🔮 Future Export Targets

TensorBoard Logs

GitHub Markdown Blocks

Quantum Graph Database Format

Plugin-as-a-Service Snapshots

REPL-to-Colab Export



---

🧠 Final Tips

Every AST pass can tag for export

Plugins can auto-export post-run

Export can be parallelized

Exporters are great for debugging!



---

> Export your logic. Export your quantum dreams. Export your entire session to a timeline on the moon.





