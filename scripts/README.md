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
# 🛠 SynQ Automation Scripts

> Every tool you need to compile, test, mutate, deploy and rebalance entropy in SynQ.

## Build & Clean
- `build.sh`: Build the compiler
- `clean.sh`: Remove builds
- `build_all_targets.sh`: Compile LLVM, C++, QASM, JSON

## Test & Coverage
- `test.sh`: Run all tests
- `coverage.sh`: Generate reports
- `fuzz_test.sh`: Fuzz plugins

## Sign & Mutate
- `sign.sh`: Quantum-sign all outputs
- `mutate.sh`: AI-mutates plugins
- `entropy_rebalance.sh`: Normalize entropy domain-wide

## Deploy & Package
- `deploy.sh`: Upload builds
- `push_synqhub.sh`: Push to SynQHub
- `package_bundle.sh`: Zip outputs

## Dev Tools
- `format.sh`: Code formatter
- `lint.sh`: Linter & static checker
- `inspect_project.sh`: Full tree analysis
- `ai_review.sh`: AI reviews plugin structures

## CI/CD
- `ci-pipeline.yml`: GitHub Actions pipeline
- `auto_commit.sh`: Stage + push auto-commits