# SynQ Review Evidence — 13 August 2026

> This record captures local command outcomes from the recovery review. It is not a release certification, performance benchmark, security assessment, or claim that untested subsystems work.

## Environment

The reviewed checkout was `/home/ubuntu/SynQ` on Ubuntu 24.04. The review began from Git revision `b4ae020` with local recovery changes not yet pushed. The native validation environment used CMake 3.28, GCC 13, `nlohmann-json3-dev`, and `libssl-dev`. `pybind11` was not required for the default profile because Python bindings are disabled there.

## Observed commands and results

| Area | Command shape | Observed outcome | Boundary |
|---|---|---|---|
| Compiler configuration | `cmake -S compiler -B /tmp/synq-compiler-final -DCMAKE_BUILD_TYPE=Release` | Configured successfully with the recovery-profile defaults | Optional CLI, legacy tests, bindings, REPL, runtime, and experimental components are not in this profile |
| Compiler core build | `cmake --build /tmp/synq-compiler-final --parallel 2` | Completed and produced `libsynq_lib.a` plus the smoke-test executable | This is a static-library build, not a verified end-user compiler command |
| Compiler smoke test | `ctest --test-dir /tmp/synq-compiler-final --output-on-failure` | Passed: 1/1 `synq_core_smoke` | The test is limited to binary-format round-trip and malformed-input bounds behavior |
| Repository frontend installation | `pnpm install --prefer-offline` in `frontend/` | Completed after cached packages were available | Installation emitted a Vite peer-dependency warning; it does not establish backend integration |
| Repository frontend type-check | `pnpm run check` in `frontend/` | Completed successfully | Historical unconfigured test sources are excluded from this application check |
| Repository frontend build | `pnpm run build` in `frontend/` | Completed successfully | The output issued a chunk-size warning; no browser flow or API integration was exercised |
| Showcase type-check and build | `pnpm exec tsc --noEmit && pnpm run build` in `synq_expansion_showcase/` | Completed successfully | This verifies the public status page builds, not the SynQ repository frontend's product behavior |
| Showcase browser review | `http://localhost:3000/` | Rendered the experimental-status homepage and no longer displayed production-readiness, historical metric, or hosted-service claims | Local preview only; no public deployment was performed |
| Docker Compose | `docker compose config --quiet` | Not run because Docker is not installed in the review environment | Compose remains unverified |

## Automated workflow status

The repository contains `.github/workflows/compiler-core.yml`. It uses Ubuntu, installs the same native dependencies, configures the recovery profile, builds it, and runs CTest. Its first observed remote result, [Compiler Core #1](https://github.com/TangoSplicer/SynQ/actions/runs/31688057478), completed successfully for commit `38e3c41` in 51 seconds. The run confirms the configured recovery profile only.

## Changes made after evidence collection

The review also corrected the native dependency declaration, added a focused compiler smoke test, repaired the repository frontend's Vite/TypeScript setup, removed stale frontend compatibility wiring, rewrote unsupported hybrid-example marketing claims as experimental design guidance, and replaced the showcase's production-readiness messaging with a factual recovery status.

## Remaining verification work

The backend remains blocked on missing local modules. Historical frontend tests are not an accepted test suite and require deliberate repair, replacement, or removal. The compiler's optional targets, end-to-end parser/runtime execution, containerized deployment, external providers, quantum hardware, performance, accuracy, security, and coverage are all outside the scope of this record.
