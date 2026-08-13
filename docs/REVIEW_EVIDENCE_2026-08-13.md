# SynQ Review Evidence — 13 August 2026

> This record captures local command outcomes from the recovery review. It is not a release certification, performance benchmark, security assessment, or claim that untested subsystems work.

## Environment

The reviewed checkout was `/home/ubuntu/SynQ` on Ubuntu 24.04. The review began from Git revision `b4ae020` with local recovery changes not yet pushed. The native validation environment used CMake 3.28, GCC 13, `nlohmann-json3-dev`, and `libssl-dev`. `pybind11` was not required for the default profile because Python bindings are disabled there.

## Observed commands and results

| Area | Command shape | Observed outcome | Boundary |
|---|---|---|---|
| Compiler configuration | `cmake -S compiler -B /tmp/synq-expanded-final -DCMAKE_BUILD_TYPE=Release` | Configured successfully with the recovery-profile defaults | Optional CLI, legacy tests, bindings, REPL command sources, and experimental components remain outside the supported profile |
| Compiler core build | `cmake --build /tmp/synq-expanded-final --parallel 2` | Completed and produced `libsynq_lib.a` plus two smoke-test executables | This is a static-library build, not a verified end-user compiler command |
| Compiler smoke suite | `ctest --test-dir /tmp/synq-openqasm3 --output-on-failure` | Passed: 3/3 `synq_core_smoke`, `synq_parser_runtime_smoke`, and `synq_openqasm3_exporter_smoke` | The parser fixture covers only a deliberately bounded line-oriented recovery grammar; the exporter fixture checks five fixed quantum kernels and rejects unsupported recovery statements; no external OpenQASM parser, SDK, or hardware is involved |
| Repository frontend installation | `pnpm install --frozen-lockfile --prefer-offline` in `frontend/` | Completed successfully | This proves the checked lockfile is reproducible in the local cached environment; it does not establish backend integration |
| Repository frontend type-check | `pnpm run check` in `frontend/` | Completed successfully | The check includes the repaired Vitest test sources and application code |
| Repository frontend test suite | `pnpm test` in `frontend/` | Passed: 3 files, 33 tests | WebSocket, GraphQL, and catalog-client tests use deterministic local mocks; the result is not a backend integration test |
| Repository frontend build and catalog browser contract | `VITE_SYNQ_API_BASE_URL=http://localhost:8000 pnpm run dev` plus local browser review | Rendered all four source-only catalog metadata records from the local FastAPI service | Lazy route/example boundaries and vendor chunks removed Vite's former 500 kB chunk warning; this is a configured local browser contract only, not a deployed backend integration |
| Backend recovery surface | `PYTHONPATH=backend python3 backend/tests/health_smoke.py` | Completed successfully | Root, health, and source-only catalog list/filter/detail/error contracts are implemented and tested; historical routers and service modules remain unavailable |
| Showcase type-check and build | `pnpm exec tsc --noEmit && pnpm run build` in `synq_expansion_showcase/` | Completed successfully | This verifies the public status page builds, not the SynQ repository frontend's product behavior |
| Showcase browser review | `http://localhost:3000/` | Rendered the experimental-status homepage and no longer displayed production-readiness, historical metric, or hosted-service claims | Local preview only; no public deployment was performed |
| Docker Compose | `docker compose config --quiet` | Not run: the review environment returned `DOCKER_UNAVAILABLE` when checked for Docker and Compose | Compose remains unverified; run this exact command in a Docker-capable environment before attempting a container startup |

## Automated workflow status

The repository contains `.github/workflows/compiler-core.yml`. It uses Ubuntu, installs the same native dependencies, configures the recovery profile, builds it, and runs CTest. Its first observed remote result, [Compiler Core #1](https://github.com/TangoSplicer/SynQ/actions/runs/31688057478), completed successfully for commit `38e3c41` in 51 seconds; [Compiler Core #2](https://github.com/TangoSplicer/SynQ/actions/runs/31693461955) also passed for the expanded fixture revision `5c11c36`; [Compiler Core #3](https://github.com/TangoSplicer/SynQ/actions/runs/31703637352) passed in 47 seconds for parser/catalog/frontend revision `21c93f3`; [Compiler Core #4](https://github.com/TangoSplicer/SynQ/actions/runs/31706565783) passed for comment-parser revision `9a7e909`; and [Compiler Core #5](https://github.com/TangoSplicer/SynQ/actions/runs/31707219450) passed in 44 seconds for OpenQASM exporter revision `f570cc9`. These runs confirm the configured recovery profile only. The frontend workflow ran frozen installation, TypeScript checking, Vitest, and build steps successfully in [Frontend #2](https://github.com/TangoSplicer/SynQ/actions/runs/31693553476) for revision `ce05438`; the initial catalog integration exposed a missing catalog-client source file because of a broad `lib/` ignore rule; and the repaired [latest frontend run](https://github.com/TangoSplicer/SynQ/actions/runs/31706700266) passed for revision `7511e81` after the rule was narrowed and the client was tracked.

## Changes made after evidence collection

The review also corrected the native dependency declaration, added focused compiler smoke tests, introduced a bounded parser representation and parser/runtime fixture, repaired the repository frontend's Vite/TypeScript and pnpm workspace setup, and replaced stale external-service test assumptions with deterministic local test fixtures. It corrected OT redo bookkeeping, added frontend CI, recovered a bounded FastAPI root/health/source-only-catalog surface and smoke test, added limited declaration and comment parser fixtures, split frontend routes, examples, and vendor code, rewrote the repository frontend and showcase messaging to match verified recovery status, and added a fixture-tested OpenQASM 3 source-export subset.

## Remaining verification work

The backend is intentionally limited to root, health, and source-only catalog endpoints; its historical routers, database integration, GraphQL, WebSocket, ML, and collaboration modules remain unavailable. The compiler's optional targets, end-to-end language execution beyond the bounded parser/runtime fixture, containerized deployment, external providers, quantum hardware, performance, accuracy, security, and whole-project coverage are all outside the scope of this record.
