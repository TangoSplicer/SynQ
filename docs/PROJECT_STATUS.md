# SynQ Project Status

**Review date:** 13 August 2026
**Review lineage:** Recovery work began from `b4ae020`; the original recovery baseline is tagged `v1.0.0-review`, and later verification updates are recorded in this status file and the evidence log.

> This file is the current operational-status record. It deliberately distinguishes **source present** from **capability verified**.

## Verified Review Results

| Area | Review outcome | Evidence and boundary |
|---|---|---|
| Core compiler library | **Builds in the recovery profile** | On Ubuntu 24.04 with CMake 3.28, GCC 13, `nlohmann-json3-dev`, and `libssl-dev`, `cmake -S compiler -B <build>` followed by `cmake --build <build>` produced `libsynq_lib.a`. The recovery profile excludes incomplete optional targets. `pybind11` is required only for the optional binding target. |
| Compiler CLI | **Not verified** | The checked-in command-line entry point is disabled by default because it has unresolved API and syntax errors. No `synq` command-line executable is claimed. |
| Compiler smoke suite | **Passes in the recovery profile** | `ctest --test-dir <build> --output-on-failure` completed with 5/5 focused smoke tests passing. They cover binary-format round-trip and malformed-input bounds handling; a bounded line-oriented recovery parser with declarations, comments, explicit `q[index]` operands, and literal-angle parameter syntax; runtime placeholder paths; strict OpenQASM 3 export; reference-parser acceptance; and Qiskit downstream conversion of the generated fixture. The parser grammar is intentionally narrow and is not a complete SynQ language implementation. |
| OpenQASM 3 source export | **Passes locally for a bounded subset with two external validation paths** | `synq_openqasm3_exporter_smoke` verifies exact OpenQASM 3 output for `h`, `x`, `y`, `z`, `cx`, `bell_pair`, and literal-angle `rx`, `ry`, `rz`, and `p` gates, including explicit qubit operands, allocation, and diagnostic rejection of unsupported statements or malformed operands. `synq_openqasm3_reference_parse` validates parser acceptance with pinned `openqasm3[parser]==1.0.1`; `synq_openqasm3_qiskit_import` imports the same output with `qiskit-qasm3-import==0.6.0` and checks its expected circuit shape. This does not establish semantic equivalence, execute, or submit to a provider. |
| Legacy compiler tests | **Not verified** | Legacy test sources fail when enabled and are disabled by default. No suite-wide pass count, coverage percentage, or performance result is claimed by this record. |
| Python bindings | **Not verified** | The binding target is disabled by default. A JSON-conversion repair was made, but the optional target still needs dedicated build and test coverage. |
| Frontend dependency configuration | **Installs locally with a frozen lockfile** | `pnpm install --frozen-lockfile --prefer-offline` completes after moving the package override to `pnpm-workspace.yaml`, regenerating the lockfile, and removing an unused Vite plugin with an incompatible peer range. |
| Frontend application type-check and build | **Pass locally; optional catalog contract browser-checked** | `pnpm run check` and `pnpm run build` both complete. Route and interactive-example boundaries are lazy-loaded, and explicit vendor chunking reduced the former single 689 kB JavaScript bundle to a 25 kB home chunk plus separate route/vendor chunks; Vite no longer emits its 500 kB chunk warning. With `VITE_SYNQ_API_BASE_URL=http://localhost:8000`, browser review confirmed that the UI renders all four source-only catalog records served by the local recovery backend. No deployed backend connection is claimed. |
| Frontend automated tests | **Pass locally** | `pnpm test` runs 3 test files with 33 passing tests. The suite imports Vitest explicitly, uses deterministic local WebSocket and `fetch` fixtures instead of unavailable services, verifies GraphQL-client fallback behavior, covers an OT redo correction, and verifies configured/unconfigured/malformed source-only catalog responses. These are local unit fixtures, not end-to-end backend integration tests. |
| Backend recovery service | **Health and source-only catalog pass locally** | The backend exposes `GET /`, `GET /health`, `GET /api/v1/examples`, and `GET /api/v1/examples/{example_id}`. `PYTHONPATH=backend python3 backend/tests/health_smoke.py` verifies health plus list, filter, detail, and error contracts through HTTPX ASGI transport. The example route exposes metadata only and explicitly labels all records `source-only-unverified`; it does not compile or execute them. Historical routers, database wiring, GraphQL, WebSocket, ML, and collaboration features remain unavailable. |
| Docker Compose | **Experimental; not verified** | Compose paths and Dockerfile paths point to the existing `backend/` and `frontend/` directories. The review environment returned `DOCKER_UNAVAILABLE`, so neither syntax validation nor an end-to-end container run was possible. In a Docker-capable environment, run `docker compose config --quiet` before attempting `docker compose up --build`. |
| Compiler-core CI workflow | **Passes on GitHub Actions** | `.github/workflows/compiler-core.yml` configures Ubuntu-based build and smoke-test steps for the verified recovery profile. [Compiler Core #6](https://github.com/TangoSplicer/SynQ/actions/runs/31710413666) completed successfully in 65 seconds for revision `d2168de`, including the explicit-qubit exporter fixture and external reference-parser test. This does not validate optional compiler targets. |
| Frontend CI workflow | **Passes on GitHub Actions** | `.github/workflows/frontend.yml` runs frozen dependency installation, TypeScript checking, Vitest, and production build steps on frontend changes. An initial catalog-integration run exposed that a broad `lib/` ignore rule had omitted the catalog client from Git. After narrowing that rule and adding the source file, the [latest observed frontend validation run](https://github.com/TangoSplicer/SynQ/actions/runs/31706700266) completed successfully for revision `7511e81`. |
| Quantum hardware, hosted services, collaboration, AI automation | **Not verified** | The presence of source files, names, examples, or older documentation does not establish that these capabilities are available or supported. |

## Changes Made During This Review

The recovery work removed a stale pnpm patch reference that prevented frontend installation, added Vite and TypeScript configuration for the actual frontend layout, corrected Docker and Compose paths, and added minimal backend dependency metadata. The repository frontend now installs from a frozen lockfile, type-checks, runs 33 local unit tests, and builds; the former backend-dependent test assumptions were replaced with deterministic client fixtures, and OT redo bookkeeping was corrected. It now lazy-loads non-home routes and interactive examples, emits separate vendor chunks without the previous Vite size warning, presents factual recovery-status messaging, and can render the local source-only catalog only when an explicit backend URL is configured. The backend has a bounded root/health surface, a source-only example catalog with tested list/filter/detail/error behavior, and an executable smoke test. The compiler CMake configuration has explicit optional switches, explicit OpenSSL linkage, and a default core-library profile that avoids known-incomplete subsystems. The recovery parser supports limited declarations plus whitespace-delimited comments, with parser/runtime smoke coverage. A strict OpenQASM 3 source exporter now provides the first verified interoperability seam for five fixed quantum kernels; it rejects all other recovery statements and makes no execution or SDK claim. Compiler-core and frontend CI workflows are defined.

Generated compiler build files, coverage output, and local IDE metadata have been removed from version control and ignored going forward.

## What Is Not Claimed

This project currently makes no verified claim of a stable release, production deployment, complete API, complete language implementation, working IDE, complete project-wide test suite, coverage percentage, performance benchmark, real quantum-hardware execution, external provider integration, commercial support SLA, or enterprise readiness.

## Recovery Priorities

The next practical phase is **reproducible baseline engineering**:

1. Observe the expanded compiler and frontend CI workflow results after this increment is pushed, and keep both workflows green over subsequent changes.
2. Extend the parser/runtime boundary incrementally—for example, comments or strict statement terminators—adding a fixture before each claim.
3. Add one bounded backend feature beyond the source-only catalog only after defining its data model, route contract, and tests; do not restore historical routers wholesale.
4. Run an end-to-end container check when a Docker-capable environment is available.
5. Repair or remove stale optional compiler targets one subsystem at a time, promoting only features with a documented command, test, and expected output.

## Historical Documentation

Older phase, architecture, deployment, and feature documents are retained for context. They may contain plans, prototypes, or claims that have not been independently verified. Treat them as historical design material unless they are explicitly updated and linked from this page.
