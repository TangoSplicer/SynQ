# SynQ

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![GitHub Discussions](https://img.shields.io/github/discussions/TangoSplicer/SynQ)](https://github.com/TangoSplicer/SynQ/discussions)

> **SynQ is an experimental, in-progress repository for a hybrid quantum, classical, and AI-oriented programming-language prototype.** It is not currently a verified end-to-end production release.

## Current Status

The repository contains a C++ compiler codebase, a React/TypeScript frontend, and a Python/FastAPI backend prototype. A technical review performed against the checked-in source found that the default compiler build, backend startup path, container deployment, and frontend dependency installation required repair before they could be presented as working product capabilities.

| Area | Current position | What is verified in this repository |
|---|---|---|
| Compiler | **Prototype; repair in progress** | CMake-based C++ source tree and example/test fixtures are present. A clean default build is being restored; the full test target remains opt-in because legacy test sources do not compile. |
| Frontend | **Prototype; repair in progress** | React/TypeScript source and a pnpm lockfile are present. The current work restores a reproducible install and Vite configuration. |
| Backend | **Prototype; incomplete** | FastAPI source and some route modules are present, but the checked-in entry point imports modules that are not currently in the backend tree. It is not yet a verified runnable API service. |
| Quantum execution | **Simulator/prototype only** | Do not infer hardware-provider support, hardware execution, or validated algorithm performance from the current repository. |
| Tests | **Not yet independently verified** | Test sources exist, but the repository does not currently provide evidence for a complete passing suite, coverage percentage, or published benchmark results. |

## What SynQ Is Exploring

SynQ explores a single-language workflow for classical logic, quantum-oriented syntax, and AI-related tooling. The source tree includes compiler components, simulation-related code, examples, web UI components, and backend route prototypes. These components are a basis for continued engineering work; they should not be treated as a supported SDK or commercial platform today.

## Repository Layout

| Path | Purpose |
|---|---|
| `compiler/` | C++ compiler and runtime prototype, CMake configuration, examples, and test fixtures. |
| `frontend/` | React/TypeScript user-interface prototype, built with Vite and pnpm. |
| `backend/` | Python/FastAPI service prototype. Some dependencies and modules remain to be completed. |
| `examples/` | SynQ example material. Treat examples as illustrative until they are covered by a passing build-and-run check. |
| `docs/` | Project documentation, including GitHub setup and launch material. |
| `scripts/` | Repository utility scripts; inspect each script before use. |

## Working With the Repository

### Clone

```bash
git clone https://github.com/TangoSplicer/SynQ.git
cd SynQ
```

### Frontend

The frontend uses pnpm. Its currently supported validation path is:

```bash
cd frontend
pnpm install --frozen-lockfile
pnpm run check
pnpm run build
```

### Compiler Prototype

The C++ project requires CMake, a C++17 compiler, `nlohmann_json`, and OpenSSL development headers. `pybind11` is required only when enabling the optional Python-binding target. The default build is a **core-library recovery profile**. It excludes legacy tests, optional Python bindings, the unfinished command-line executable, experimental REPL command sources, and debugger/network/optimizer/plugin/runtime subsystems.

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel
```

Use `-DBUILD_TESTS=ON`, `-DBUILD_PYTHON_BINDINGS=ON`, `-DBUILD_CLI_TOOLS=ON`, `-DBUILD_COMPILER_EXECUTABLE=ON`, `-DBUILD_REPL=ON`, or `-DBUILD_EXPERIMENTAL_COMPONENTS=ON` only when actively repairing those targets. None is currently represented as a passing supported workflow.

### Backend Prototype

The backend is **not currently documented as runnable**. Its source imports modules that are absent from the repository, and it needs a focused recovery pass before a run command can be provided responsibly.

## Documentation

| Document | Purpose |
|---|---|
| [GETTING_STARTED.md](GETTING_STARTED.md) | Verified setup boundaries and first contribution steps. |
| [USER_GUIDE.md](USER_GUIDE.md) | Current capability and limitation guide. |
| [CONTRIBUTING.md](CONTRIBUTING.md) | Contribution process. |
| [SECURITY.md](SECURITY.md) | Security-reporting policy. |
| [CHANGELOG.md](CHANGELOG.md) | Documented project history. |
| [docs/github-setup/](docs/github-setup/) | Optional GitHub administration guides. |

## Roadmap

The immediate roadmap is recovery and verification, not feature expansion.

1. Establish reproducible frontend, compiler, and backend build paths.
2. Repair or retire stale test targets and publish real test outcomes.
3. Stabilize a small, documented language subset with examples that run in CI.
4. Restore the backend only after its required modules, dependencies, and tests are present.
5. Evaluate future quantum, AI, collaboration, and hardware features through scoped issues and demonstrated implementations.

Features such as hardware integration, AI-driven circuit synthesis, real-time collaboration, package registries, production SLAs, performance guarantees, and commercial support are **not current, verified capabilities** of this repository.

## Contributing

Contributions that improve reproducibility, build health, tests, minimal examples, documentation, and issue triage are especially valuable. Please read [CONTRIBUTING.md](CONTRIBUTING.md), check existing issues, and open a discussion before beginning a large change.

## Community

- [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions) for questions and ideas.
- [GitHub Issues](https://github.com/TangoSplicer/SynQ/issues) for reproducible bugs and scoped feature requests.

## Licensing

The repository includes an MIT license file and separate commercial-license materials. Read the actual terms in [LICENSE](LICENSE), [LICENSE_COMMERCIAL.md](LICENSE_COMMERCIAL.md), and [LICENSE_FAQ.md](LICENSE_FAQ.md) before relying on either path. This README does not modify those terms.

## Transparency Note

This README intentionally distinguishes checked-in source from verified product functionality. If you find a claim that conflicts with the code or a reproducible result, please open an issue with the relevant command and output.
