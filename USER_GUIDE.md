# SynQ User Guide

## Status

SynQ is an experimental hybrid-programming-language repository. Its checked-in source contains a C++ compiler prototype, a React/TypeScript frontend prototype, and a Python/FastAPI backend prototype. These components are at different levels of completeness and are not currently verified as a complete end-to-end product.

This guide therefore describes how to interpret the repository safely. It does not claim package-manager installation, a production IDE, quantum-hardware access, validated FFI support, benchmarked performance, complete ML support, complete API coverage, or an operational enterprise service.

## Repository Components

| Component | Intended role | Present limitation |
|---|---|---|
| `compiler/` | C++ implementation of language, runtime, simulation, and tooling experiments. | Full legacy tests and optional Python bindings need repair. Build success must be established per revision. |
| `frontend/` | Browser-based interface prototype. | It does not establish the availability of the backend services or UI-advertised advanced features. |
| `backend/` | FastAPI service prototype. | The current application entry point references missing local modules and is not a supported service. |
| `examples/` | Illustrative `.synq` programs and supporting material. | Examples are not a substitute for a passing execute-in-CI validation. |

## Safe Usage Model

Treat SynQ as source to explore, audit, and improve. Begin with the current [Getting Started guide](GETTING_STARTED.md), build only the verified default compiler profile, and record the exact commands and outputs you observe. For an example to be considered working, it should have a reproducible build-and-run command and an automated test.

## Quantum and AI Claims

The repository contains quantum- and AI-related source, examples, and design material. This demonstrates areas of investigation, not guaranteed end-user functionality. In particular, do not infer current support for real hardware providers, hardware-aware transpilation, automatic circuit synthesis, production machine-learning models, real-time collaboration, package registries, cloud deployment, or benchmarked optimisation from directory names or old documentation.

## Reporting a Problem

Include the following in an issue whenever possible:

1. Operating system and version.
2. CMake, compiler, Node.js, pnpm, and Python versions.
3. The exact command run from the repository root.
4. Complete relevant output, with credentials removed.
5. The smallest source file or reproduction steps that show the issue.

This information makes a report actionable for a solo maintainer and future contributors.

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a pull request. Prioritise changes that make the repository more reproducible, such as restoring a build path, adding a focused test, correcting a misleading claim, or removing generated artifacts from version control.

## Licensing

Refer to [LICENSE](LICENSE), [LICENSE_COMMERCIAL.md](LICENSE_COMMERCIAL.md), and [LICENSE_FAQ.md](LICENSE_FAQ.md) for the actual licensing terms. Do not rely on statements from prior documentation or social posts where they conflict with those files.
