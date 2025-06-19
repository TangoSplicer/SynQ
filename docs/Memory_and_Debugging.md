// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

# Memory and Debugging

## 📜 Agentic Memory
Track agent plans, errors, decisions:
```synq
mem = new_memory("id")
mem = remember(mem, "plan", "run qaoa")
```

## 🧠 Retrieval
```synq
recall_by_label(mem, "plan1")
```

## 🔍 Profiler
Profile runtime blocks:
```synq
@profiler_start("block")
...
@profiler_end("block")
```

## 👁️ Visualizer
Visual annotations:
```synq
@visual_start
H(0)
@visual_end("id", title="Circuit")
```
