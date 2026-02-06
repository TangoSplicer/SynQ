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



