# Tested Environments and Clean Installation

**Status:** Evidence record for the experimental compiler recovery profile and
static C ABI SDK. This document defines the environments that have been tested;
it is not a cross-platform support promise.

## Verified environment boundary

The recovery compiler profile is continuously exercised by the
[`Compiler Core` workflow](../.github/workflows/compiler-core.yml) on GitHub's
`ubuntu-latest` runner. The most recent full-profile evidence is [Compiler Core
#50](https://github.com/TangoSplicer/SynQ/actions/runs/31956231719), which
completed successfully with **27/27** CTest checks for revision `addad26`.

The same workflow now includes a distinct `ubuntu-22.04` job that installs a
minimal static SDK into a fresh temporary prefix and builds/runs the external C
header consumer through `CMAKE_PREFIX_PATH`. That second job also passed in
[Compiler Core #50](https://github.com/TangoSplicer/SynQ/actions/runs/31956231719).

The local confirmation made on 16 August 2026 used the following Ubuntu 24.04
environment. It rebuilt the same recovery profile and independently passed the
installed-SDK conformance test. These local tool versions are informative
evidence, not minimum-version guarantees.

| Component | Local confirmation | CI installation/source |
| --- | --- | --- |
| Operating system | Ubuntu 24.04 | GitHub `ubuntu-latest` recovery profile and fixed `ubuntu-22.04` static-SDK conformance job |
| CMake | 3.28.3 | `apt` `cmake`; project requires CMake 3.18 or later |
| C++ compiler | GCC 13.3.0 | `apt` `g++`; project requires C++17 support |
| OpenSSL | 3.0.13 | `apt` `libssl-dev` |
| JSON | `nlohmann-json3-dev` 3.11.3 | `apt` `nlohmann-json3-dev` |
| Interop fixtures | Rust, SBCL/CFFI, Clojure/JNA, Mercury 22.01.8 | Installed or bootstrapped by Compiler Core |

> **Support boundary:** The recovery profile has remote evidence on
> `ubuntu-latest`; the experimental static SDK clean-install path has an
> additional fixed Ubuntu 22.04 result. macOS, Windows, other Linux
> distributions, alternate compilers, ARM hosts, dynamic linking, package
> registries, and ABI compatibility across future revisions remain unvalidated.

## Reproduce the supported recovery profile

On a Debian/Ubuntu-like host, install the native dependencies before configuring
the bounded profile. The command is designed for the source-based experimental
workflow; it does not install a stable system package.

```bash
sudo apt-get update
sudo apt-get install -y \
  cmake g++ rustc cargo sbcl cl-cffi clojure libjna-java \
  flex bison nlohmann-json3-dev libssl-dev

python3 -m pip install --disable-pip-version-check \
  -r compiler/tests/requirements-openqasm3.txt

cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTS=OFF \
  -DBUILD_PYTHON_BINDINGS=OFF \
  -DBUILD_CLI_TOOLS=OFF \
  -DBUILD_COMPILER_EXECUTABLE=OFF \
  -DBUILD_EXPERIMENTAL_COMPONENTS=OFF \
  -DBUILD_REPL=OFF \
  -DBUILD_CORE_SMOKE_TESTS=ON

cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
```

The Mercury fixture requires its cached/toolchain setup path used in the CI
workflow. Follow `compiler/tests/interop/setup_mercury_toolchain.sh` rather than
assuming a distribution package provides an equivalent compiler.

## Experimental native SDK clean-install check

The recovery profile includes `synq_installed_sdk_conformance`. It performs the
following actions in build-local temporary directories:

| Step | Verification |
| --- | --- |
| Install | `cmake --install` places the static library, public header, CLI, documentation, and CMake package in an isolated prefix. |
| Discover | A separate CMake project invokes `find_package(SynQ CONFIG REQUIRED)` using only that prefix. |
| Compile and link | A C source includes only `<synq/synq_ffi.h>` and links the C++-implemented static library through `SynQ::synq_lib`. |
| Execute | The consumer checks ABI identity, parses bounded in-memory source, exports OpenQASM 3, and frees all library-owned outputs. |

Run only this check with:

```bash
ctest --test-dir compiler/build \
  -R synq_installed_sdk_conformance --output-on-failure
```

The installed path is an **experimental static-library integration mechanism**.
It does not supply a stable shared library, binary compatibility commitment,
language wrapper, registry package, cryptographic signature, provenance
attestation, or operating-system support guarantee.

The fixed Ubuntu 22.04 job is described in
[`SECOND_CLEAN_INSTALL_ENVIRONMENT.md`](./SECOND_CLEAN_INSTALL_ENVIRONMENT.md).
It validates the same static path in a separate environment; it does not broaden
the support boundary beyond two Ubuntu-based CI environments.

## Reporting an environment problem

When the recovery profile fails, open a GitHub issue with the host OS/release,
CMake and compiler versions, complete configuration command, first failing CTest
output, and whether the failure occurs before or after the clean-install check.
Do not report untested platforms as regressions until a maintainer reproduces or
adds them to the verified matrix.
