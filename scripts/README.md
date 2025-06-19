// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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