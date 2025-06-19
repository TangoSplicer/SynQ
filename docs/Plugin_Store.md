// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

# Compiler Plugin Store

Extend SynQ with your own plugins!

## 🔌 Registering a Plugin
```synq
register_plugin("double", "1.0", "doubles input", (x) => x.value * 2)
```

## ▶️ Invoking a Plugin
```synq
invoke_plugin("double", { "value": 21 })  // 42
```

## 🧠 Use Cases
- AST transformers
- Debug utilities
- External model invokers

