# SynQ Project Status

**Review date:** 13 August 2026
**Repository revision reviewed:** `38e3c41` (`Recover build baseline and add compiler core CI`), tagged `v1.0.0-review`.

> This file is the current operational-status record. It deliberately distinguishes **source present** from **capability verified**.

## Verified Review Results

| Area | Review outcome | Evidence and boundary |
|---|---|---|
| Core compiler library | **Builds in the recovery profile** | On Ubuntu 24.04 with CMake 3.28, GCC 13, `nlohmann-json3-dev`, and `libssl-dev`, `cmake -S compiler -B <build>` followed by `cmake --build <build>` produced `libsynq_lib.a`. The recovery profile excludes incomplete optional targets. `pybind11` is required only for the optional binding target. |
| Compiler CLI | **Not verified** | The checked-in command-line entry point is disabled by default because it has unresolved API and syntax errors. No `synq` command-line executable is claimed. |
| Compiler smoke test | **Passes in the recovery profile** | `ctest --test-dir <build> --output-on-failure` completed with 1/1 focused smoke test passing. The test covers binary-format round-trip and malformed-input bounds handling only. |
| Legacy compiler tests | **Not verified** | Legacy test sources fail when enabled and are disabled by default. No suite-wide pass count, coverage percentage, or performance result is claimed by this record. |
| Python bindings | **Not verified** | The binding target is disabled by default. A JSON-conversion repair was made, but the optional target still needs dedicated build and test coverage. |
| Frontend dependency configuration | **Installs locally** | `pnpm install --prefer-offline` completed after resolving the stale `wouter` patch metadata. One peer-dependency warning remains: `@builder.io/vite-plugin-jsx-loc` declares Vite 4/5 compatibility while the project uses Vite 7. |
| Frontend application type-check and build | **Pass locally** | `pnpm run check` and `pnpm run build` both completed after adding Vite environment types, correcting a duplicate/missing `node:path` import, and removing an unused re-export to a non-existent shared module. This verifies compilation and bundling only; it does not verify backend-connected user flows. |
| Frontend automated tests | **Not verified** | Historical `client/src/__tests__` files are excluded from the application type-check because there is no configured test command and the files contain stale expectations for unavailable backend services. No frontend test-suite pass count is claimed. |
| Backend service | **Not runnable as checked in** | `backend/app/main.py` imports local modules such as `app.config`, `app.database`, and several routers that are absent from the current backend tree. The backend must be recovered before it is offered as an API service. |
| Docker Compose | **Experimental; not verified** | Compose paths and Dockerfile paths were corrected to point to the existing `backend/` and `frontend/` directories. The frontend now builds locally, but an end-to-end container run remains blocked by backend recovery and Docker is not installed in the review environment. |
| Compiler-core CI workflow | **Defined and pushed; remote result not recorded here** | `.github/workflows/compiler-core.yml` configures Ubuntu-based build and smoke-test steps for the verified recovery profile. The workflow was pushed with revision `38e3c41`; check the GitHub Actions page for its current or subsequent run result. |
| Quantum hardware, hosted services, collaboration, AI automation | **Not verified** | The presence of source files, names, examples, or older documentation does not establish that these capabilities are available or supported. |

## Changes Made During This Review

The recovery work removed a stale pnpm patch reference that prevented frontend installation, added Vite and TypeScript configuration for the actual frontend layout, corrected Docker and Compose paths, and added minimal backend dependency metadata. The repository frontend now installs, type-checks, and builds locally; the application check deliberately excludes unconfigured historical tests that depend on unavailable services. The compiler CMake configuration now has explicit optional switches, explicit OpenSSL linkage, and a default core-library profile that avoids known-incomplete subsystems. Several source-level compiler errors discovered during the build were corrected, including missing standard includes, metadata accessors, constness mismatches, and private-state access. A focused binary-format smoke test and its corresponding GitHub Actions workflow were added.

Generated compiler build files, coverage output, and local IDE metadata have been removed from version control and ignored going forward.

## What Is Not Claimed

This project currently makes no verified claim of a stable release, production deployment, complete API, complete language implementation, package-manager installation, working IDE, passing test suite, coverage percentage, performance benchmark, real quantum-hardware execution, external provider integration, commercial support SLA, or enterprise readiness.

## Recovery Priorities

The next practical phase is **reproducible baseline engineering**:

1. Expand compiler smoke coverage with a small executable and one end-to-end parser/runtime test.
2. Observe the compiler-core GitHub Actions result before advertising CI status.
3. Recover the FastAPI application around a minimal health endpoint with complete local modules and tests.
4. Configure, repair, or remove stale frontend tests; then establish frontend type-check/build CI for the repository frontend.
5. Repair or remove stale optional compiler targets one subsystem at a time, promoting only features with a documented command, test, and expected output.

## Historical Documentation

Older phase, architecture, deployment, and feature documents are retained for context. They may contain plans, prototypes, or claims that have not been independently verified. Treat them as historical design material unless they are explicitly updated and linked from this page.
