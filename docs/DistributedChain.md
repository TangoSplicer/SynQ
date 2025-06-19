

📄

✅ File: docs/DistributedChain.md

# ⚙️ Distributed Execution Chain (DEC)

---

## Overview

The Distributed Execution Chain (DEC) system allows SynQ to run job DAGs composed of:

- Plugin-based operations
- Shell commands
- Remote tasks
- Quantum circuit batches

---

## File Format

Use `.chain.json` or `.yaml`:

```json
{
  "tasks": [
    { "id": "prepare", "run": "synq compile prep.synq" },
    { "id": "simulate", "run": "synq run sim_qaoa.synq", "depends_on": ["prepare"] },
    { "id": "postprocess", "run": "python3 analyze.py", "depends_on": ["simulate"] }
  ]
}


---

CLI Usage

synq chain jobs/example.chain.json


---

DAG Runtime

Each task is assigned an ID

Dependencies must be completed before execution

Output and errors are captured per node



---

Visual Execution (Notebook)

Run the DEC inside:

notebooks/55_chain_executor.ipynb


---

Integration Ideas

Trigger from synq deploy

Use plugins as task handlers

Emit telemetry to skill tracker



---

Status

✅ Available
🧠 Plugin execution support (future)
📡 Remote node chaining (future)

---

✅ **Distributed Execution Chain is now complete**:

- Engine: `execution_graph.py`
- CLI: `synq_chain.cpp`
- Notebook: `55_chain_executor.ipynb`
- Docs: `DistributedChain.md`



