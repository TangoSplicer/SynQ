// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

# AI Integration in SynQ

SynQ tightly integrates with AI systems:

## 🤖 ModelProvider
Call AI models from the DSL:
```synq
call_model("llama-mock", "Explain QFT")
```

## 🔗 AI Fusion Layer
Use AI decisions to control gates, inject values, or modify pipelines.

## 🧠 Rerankers and Tools
Use built-in tools:
```synq
@reranker.choose(options, context)
```

## 🤯 Multi-agent Capable
Each agent can have its own memory, models, tools, and goals.

