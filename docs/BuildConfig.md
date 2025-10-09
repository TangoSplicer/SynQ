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
✅ File: synq/docs/BuildConfig.md

# SynQ Build Configuration Guide

## Overview

SynQ is built with a modular and portable architecture, designed for easy compilation on Linux, macOS, and Windows. This guide details all build configuration options, profiles, environment variables, and optimization flags supported by the SynQ build system.

---

## 1. Build System

SynQ uses **CMake** for cross-platform configuration.

### Minimum Requirements
- CMake ≥ 3.21
- C++20-compatible compiler
- Python ≥ 3.10 (optional: AI/Plugin tools)
- LLVM ≥ 14 (for IR-based optimizations)

---

## 2. Building the Project

```bash
cd synq
mkdir build && cd build
cmake ..
make -j$(nproc)

Outputs:

synq_run - core compiler/interpreter

synq_repl - REPL shell

libsynq.so - shared lib (if enabled)

test_runner - test runner executable



---

3. Build Profiles

Profile	Description	Flags

Debug	For REPL, tracing, and memory logs	-DSYNQ_DEV=ON
Release	Optimized compiler runtime	-DSYNQ_RELEASE=ON
Sandbox	Secure plugin isolation	-DSYNQ_SANDBOX=ON
SharedLib	Build as library for embedding	-DBUILD_SHARED=ON


Example:

cmake .. -DSYNQ_DEV=ON -DSYNQ_SANDBOX=ON


---

4. Key CMake Flags

Flag	Purpose

BUILD_TESTING=ON	Enable test runners and coverage reports
BUILD_REPL=ON	Include REPL shell
BUILD_PLUGINS=ON	Enable plugin support
ENABLE_VISUALS=ON	Compile visual output handlers
BUILD_DOCS=ON	Include Doxygen/Markdown tools



---

5. Environment Variables

Variable	Description

SYNQ_HOME	Path to base SynQ directory
SYNQ_AI_KEY	API key for AI integrations
SYNQ_VISUAL_MODE	Enables headless rendering (true/false)
SYNQ_PLUGIN_PATH	Colon-separated paths for plugin discovery
SYNQ_CACHE_SIZE	Sets memory cache size (e.g., "256MB")



---

6. Linking Options

To link SynQ into another project:

target_link_libraries(my_app PRIVATE synq::core)

For shared usage:

cmake .. -DBUILD_SHARED=ON


---

7. Compiler Support

Compiler	Minimum Version	Notes

GCC	11.0	Required for full C++20 support
Clang	13.0	Preferred for diagnostics
MSVC	2022	Windows support (experimental)



---

8. Recommended Build Scripts

Preconfigured scripts in deploy/scripts/:

build_local.sh — basic local build

build_shared.sh — build with shared lib

docker_build_push.sh — for containerized CI/CD

cloud_deploy.sh — bundled with quantum job scaffolding



---

9. Troubleshooting

Problem	Fix

CMake can't find source	Run from synq/ root and mkdir build/
Missing plugin headers	Set SYNQ_PLUGIN_PATH correctly
Build fails on REPL	Ensure BUILD_REPL=ON and readline is installed



---

10. Future Enhancements

WASM build target (browser runtime)

Build-time plugin scoring

Auto-snapshotting build metadata for auditability

LTO + GPU-targeted backend optimizations



---

Help

cmake --help
synq config --help

Build discussions: synqhub.org/docs/build


---

© 2025 SynQ Project — Build Once, Run Everywhere (Quantum or Not).

---



