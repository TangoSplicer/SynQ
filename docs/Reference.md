// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

✅ Updated File: synq/docs/Reference.md

# 📘 SynQ Reference Manual

This file contains quick-reference tables and code snippets for all major SynQ operations.

---

## 🔹 Types and Declarations

| Keyword     | Description                     |
|-------------|---------------------------------|
| `int`       | Integer                         |
| `float`     | Floating point                  |
| `qubit`     | Quantum bit                     |
| `circuit`   | Quantum circuit block           |
| `model`     | AI model declaration            |
| `plugin`    | Plugin access/control           |

---

## 🔹 Quantum Circuit Syntax

```synq
circuit Bell {
  q0 = |0>
  h(q0)
  cnot(q0, q1)
}


---

🔹 AI Integration

model GPT {
  name: "gpt-4"
  provider: "openai"
}

explain = run GPT("What is a QFT?")


---

🔹 Plugin & Mutation System

:= load visual_debugger
:= mutate optimization_engine
:= inspect plugin


---

🔹 Inspector Tools

:inspect mem
:inspect qreg
:inspect plugin


---

🔹 Snapshot & Rollback

:snapshot
:undo
:history


---

🧠 Advanced Features

AI inline completions

Visual plotting with :plot

Causal trace viewing with :trace

Plugin signing using QRYPTA-TGIL (synq sign)

CLI mutation tools (synq mutate, synq fuzz)



---

⚠️ Licensing

This document and all code examples are part of the proprietary SynQ system.

Commercial use, distribution, or derivative work without a license is strictly prohibited.
See LICENSE.txt for details.


---



