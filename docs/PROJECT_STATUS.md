# SynQ Project Status

**Review date:** 13 August 2026
**Review lineage:** Recovery work began from `b4ae020`; the original recovery baseline is tagged `v1.0.0-review`, and later verification updates are recorded in this status file and the evidence log.

> This file is the current operational-status record. It deliberately distinguishes **source present** from **capability verified**.

## Verified Review Results

| Area | Review outcome | Evidence and boundary |
|---|---|---|
| Core compiler library | **Builds in the recovery profile** | On Ubuntu 24.04 with CMake 3.28, GCC 13, `nlohmann-json3-dev`, and `libssl-dev`, `cmake -S compiler -B <build>` followed by `cmake --build <build>` produced `libsynq_lib.a`. The recovery profile excludes incomplete optional targets. `pybind11` is required only for the optional binding target. |
| Compiler CLI | **Not verified** | The checked-in command-line entry point is disabled by default because it has unresolved API and syntax errors. No `synq` command-line executable is claimed. |
| Compiler smoke suite | **Passes in the recovery profile** | `ctest --test-dir <build> --output-on-failure` completed with 2/2 focused smoke tests passing. They cover binary-format round-trip and malformed-input bounds handling; a bounded line-oriented recovery parser; parser rejection of malformed, unsupported, and missing input; and runtime placeholder paths for print, delay, quantum, AI, and unknown instructions. The parser grammar is intentionally narrow and is not a complete SynQ language implementation. |
| Legacy compiler tests | **Not verified** | Legacy test sources fail when enabled and are disabled by default. No suite-wide pass count, coverage percentage, or performance result is claimed by this record. |
| Python bindings | **Not verified** | The binding target is disabled by default. A JSON-conversion repair was made, but the optional target still needs dedicated build and test coverage. |
| Frontend dependency configuration | **Installs locally with a frozen lockfile** | `pnpm install --frozen-lockfile --prefer-offline` completes after moving the package override to `pnpm-workspace.yaml`, regenerating the lockfile, and removing an unused Vite plugin with an incompatible peer range. |
| Frontend application type-check and build | **Pass locally** | `pnpm run check` and `pnpm run build` both completed after adding Vite environment types, correcting a duplicate/missing `node:path` import, and removing an unused re-export to a non-existent shared module. This verifies compilation and bundling only; it does not verify backend-connected user flows. |
| Frontend automated tests | **Pass locally** | `pnpm test` runs 2 test files with 30 passing tests. The suite now imports Vitest explicitly, uses deterministic local WebSocket and `fetch` fixtures instead of unavailable services, verifies GraphQL-client fallback behavior, and covers an OT redo correction. These are local unit fixtures, not end-to-end backend integration tests. |
| Backend recovery service | **Minimal health surface passes locally** | The backend now exposes only `GET /` and `GET /health`; `PYTHONPATH=backend python3 backend/tests/health_smoke.py` verifies both response contracts via HTTPX ASGI transport. Historical routers, database wiring, GraphQL, WebSocket, ML, and collaboration features remain unavailable. |
| Docker Compose | **Experimental; not verified** | Compose paths and Dockerfile paths point to the existing `backend/` and `frontend/` directories. The minimal backend service and frontend now build locally, but Docker is not installed in the review environment and no end-to-end container run is claimed. |
| Compiler-core CI workflow | **Passes on GitHub Actions** | `.github/workflows/compiler-core.yml` configures Ubuntu-based build and smoke-test steps for the verified recovery profile. Its first observed run, [Compiler Core #1](https://github.com/TangoSplicer/SynQ/actions/runs/31688057478), completed successfully for revision `38e3c41` in 51 seconds. This does not validate optional compiler targets. |
| Frontend CI workflow | **Defined locally; remote result pending push** | `.github/workflows/frontend.yml` runs frozen dependency installation, TypeScript checking, Vitest, and production build steps on frontend changes. A GitHub-hosted result must be observed after the review changes are pushed. |
| Quantum hardware, hosted services, collaboration, AI automation | **Not verified** | The presence of source files, names, examples, or older documentation does not establish that these capabilities are available or supported. |

## Changes Made During This Review

The recovery work removed a stale pnpm patch reference that prevented frontend installation, added Vite and TypeScript configuration for the actual frontend layout, corrected Docker and Compose paths, and added minimal backend dependency metadata. The repository frontend now installs from a frozen lockfile, type-checks, runs 30 local unit tests, and builds; the former backend-dependent test assumptions were replaced with deterministic client fixtures, and OT redo bookkeeping was corrected. The backend now has a bounded root/health recovery service plus an executable smoke test. The compiler CMake configuration now has explicit optional switches, explicit OpenSSL linkage, and a default core-library profile that avoids known-incomplete subsystems. The recovery parser has a limited tested instruction grammar and the smoke suite now includes a parser/runtime executable alongside the binary-format test. Several source-level compiler errors discovered during the build were corrected, including missing standard includes, metadata accessors, constness mismatches, and private-state access. Compiler-core and frontend CI workflows are defined.

Generated compiler build files, coverage output, and local IDE metadata have been removed from version control and ignored going forward.

## What Is Not Claimed

This project currently makes no verified claim of a stable release, production deployment, complete API, complete language implementation, package-manager installation, working IDE, passing test suite, coverage percentage, performance benchmark, real quantum-hardware execution, external provider integration, commercial support SLA, or enterprise readiness.

## Recovery Priorities

The next practical phase is **reproducible baseline engineering**:

1. Observe the new frontend CI workflow after the expanded recovery work is pushed, and keep both CI workflows green over subsequent changes.
2. Expand the parser/runtime boundary incrementally from the documented recovery grammar, adding a fixture before each claim.
3. Add one bounded backend feature beyond health only after its data model, route contract, and tests are implemented; do not restore historical routers wholesale.
4. Run an end-to-end container check when a Docker-capable environment is available.
5. Repair or remove stale optional compiler targets one subsystem at a time, promoting only features with a documented command, test, and expected output.

## Historical Documentation

Older phase, architecture, deployment, and feature documents are retained for context. They may contain plans, prototypes, or claims that have not been independently verified. Treat them as historical design material unless they are explicitly updated and linked from this page.
