# Getting Started with SynQ

## Scope and Current Boundaries

SynQ is an experimental repository. The project is currently in a recovery and verification phase, so this guide documents only workflows that can be checked against the repository structure. It does **not** promise a working language installation, package manager, Docker deployment, cloud service, IDE, quantum-hardware integration, performance target, or complete test suite.

## Prerequisites

The default C++ compiler recovery profile requires a C++17 compiler, CMake 3.18 or newer, `nlohmann_json`, and OpenSSL development headers. `pybind11` is additionally required only for the optional Python-binding target. The frontend prototype requires Node.js 22 or newer and pnpm 10. The backend source has a minimal dependency file, but its runtime entry point still depends on missing local modules and is therefore not a supported setup path.

On Ubuntu or Debian, the compiler prerequisites can be installed with:

```bash
sudo apt-get update
sudo apt-get install -y cmake g++ nlohmann-json3-dev libssl-dev
```

On macOS, install CMake and a compiler toolchain with Xcode Command Line Tools and Homebrew. Install `pybind11` and `nlohmann-json` through Homebrew or provide their CMake package locations explicitly. On Windows, use WSL2 and follow the Linux instructions.

## Clone the Repository

```bash
git clone https://github.com/TangoSplicer/SynQ.git
cd SynQ
```

## Validate the Frontend Prototype

Run the following commands from `frontend/`:

```bash
pnpm install --frozen-lockfile
pnpm run check
pnpm run build
```

If all three commands succeed, preview the generated static frontend:

```bash
pnpm run preview
```

The frontend is a prototype interface. Do not assume that UI controls correspond to completed backend or quantum-execution functionality unless the corresponding interaction is covered by a reproducible test.

## Build the Compiler Prototype

The default compiler profile is a core-library recovery build. It excludes legacy tests, Python bindings, the unfinished command-line executable, experimental REPL sources, and debugger/network/optimizer/plugin/runtime subsystems. Configure and build it as follows:

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel
```

The command verifies whether the checked-in C++ core compiles in your environment. A successful build does not by itself establish that all `.synq` examples run, that all language features work, or that the project has a published compiler binary. Those outcomes must be verified independently and reported with command output.

To work specifically on excluded targets, enable them explicitly:

```bash
cmake -S compiler -B compiler/build-tests -DBUILD_TESTS=ON
cmake -S compiler -B compiler/build-bindings -DBUILD_PYTHON_BINDINGS=ON
cmake -S compiler -B compiler/build-experimental -DBUILD_EXPERIMENTAL_COMPONENTS=ON
```

These targets are known to need additional repair work and are not the default developer path.

## Backend Status

The backend source is organised as a FastAPI application, and `backend/requirements.txt` records its minimal external dependencies. However, the checked-in application imports modules that are absent from the current tree. Do not run or deploy it as a working service until the backend recovery issue is completed with a passing startup check and tests.

## Docker Status

The repository includes Dockerfiles and a Compose file that are being aligned with the current `frontend/` and `backend/` directories. Containerisation remains experimental until an end-to-end `docker compose up` check is documented as passing. Do not use Docker as an installation method at this stage.

## How to Help

The most valuable early contributions are small, reproducible improvements:

| Contribution | Example evidence |
|---|---|
| Build repair | Command, operating system, tool versions, and complete error or success output. |
| Test repair | A focused test that fails before the change and passes afterward. |
| Example validation | Example source, exact compiler command, and expected output. |
| Documentation correction | A link to the claim plus the code/configuration that contradicts it. |

Use [GitHub Issues](https://github.com/TangoSplicer/SynQ/issues) for reproducible defects and [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions) for questions or proposals.
