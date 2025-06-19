// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

# Compiler Architecture

The SynQ compiler is written in C++ and includes:

## 🔧 Core Components
- parser.h / ast.h
- codegen.h / runtime.cpp

## ⚙️ Transpiler
- qtranspile system bridges DSL to backend IR

## 🔌 Plugin System
- Dynamically loads tools via plugin_store

## 🔮 Visualizer + Profiler
- Integrated into runtime and DSL with `@visual_start` and `@profiler_start`

