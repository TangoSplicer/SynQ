# Tested Environments and Clean Installation

**Status:** Evidence record for the experimental compiler recovery profile and
static C ABI SDK. This document defines the environments that have been tested;
it is not a cross-platform support promise.

## Verified environment boundary

The recovery compiler profile is continuously exercised by the
[`Compiler Core` workflow](../.github/workflows/compiler-core.yml). The latest
remote evidence is [Compiler Core platform-matrix run
#32369872013](https://github.com/TangoSplicer/SynQ/actions/runs/32369872013)
for reliability revision `2965ec4`. Its ordinary Linux job passed the full
**46/46** recovery-profile CTest suite; distinct Windows MSVC and macOS Clang
jobs each passed a **32-test** platform-neutral compiler/CLI/C-ABI smoke profile.
An additive Ubuntu Clang ASan/UBSan job separately passed **32/32** tests under
the documented sanitizer runtime settings.

The same workflow includes distinct Ubuntu 22.04, Windows MSVC, and macOS Clang
clean-install jobs. Each builds a minimal static SDK producer, installs it into a
fresh temporary prefix, configures an external public-header C consumer through
`CMAKE_PREFIX_PATH`, and executes that consumer. All three jobs passed in
[the same platform-matrix run](https://github.com/TangoSplicer/SynQ/actions/runs/32369872013).

The local confirmation made on 16 August 2026 used the following Ubuntu 24.04
environment. It rebuilt the same recovery profile and independently passed the
installed-SDK conformance test. These local tool versions are informative
evidence, not minimum-version guarantees.

| Component | Local confirmation | CI installation/source |
| --- | --- | --- |
| Operating system | Ubuntu 24.04 | GitHub `ubuntu-latest` full recovery profile, `windows-latest` MSVC platform-neutral smoke and clean-install SDK jobs, `macos-latest` Clang platform-neutral smoke and clean-install SDK jobs, and fixed `ubuntu-22.04` static-SDK conformance job |
| CMake | 3.28.3 | `apt` `cmake`; project requires CMake 3.18 or later |
| C++ compiler | GCC 13.3.0 | `apt` `g++` on Ubuntu; MSVC via `windows-latest`; Apple Clang via `macos-latest`; project requires C++17 support |
| Sanitizer compiler | Clang, isolated local profile | `apt` `clang` on `ubuntu-latest`; only the additive 32-test `SYNQ_ENABLE_SANITIZERS=ON` profile uses ASan/UBSan |
| OpenSSL | 3.0.13 | `apt` `libssl-dev` |
| JSON | `nlohmann-json3-dev` 3.11.3 | `apt` `nlohmann-json3-dev` |
| Interop fixtures | Rust, SBCL/CFFI, Clojure/JNA, Mercury 22.01.8 | Installed or bootstrapped by Compiler Core |

> **Support boundary:** `windows-latest` has remote MSVC evidence for a 32-test
> platform-neutral compiler/CLI/C-ABI smoke profile **and** an experimental
> static-SDK clean-install external-consumer check. `macos-latest` has a separate
> 32-test Clang platform-neutral smoke profile **and** an experimental static-SDK
> clean-install external-consumer check. The full 46-test suite, the
> direct Rust, source-only Alpha Rust-wrapper, Common Lisp/Clojure/Mercury fixtures,
> and OpenQASM Python reference checks
> remain Ubuntu-only evidence. These CI results do not establish a stable ABI,
> shared-library delivery, registry package, broad package support, dynamic
> linking, general cross-platform distribution, other Linux distribution, ARM, or future ABI
> compatibility claim.

> **Sanitizer boundary:** Compiler Core #32369872013 adds Linux/Clang ASan/UBSan
> evidence for a 32-test isolated core profile only. It does not instrument the
> Ubuntu full-recovery, Windows, macOS, SDK, language-interop, reference-parser,
> or release artifact paths, and it is not a security certification.

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

## Reproduce the Windows MSVC smoke profile

The Windows smoke job is deliberately separate from the Ubuntu full profile. It
installs `nlohmann-json` and OpenSSL with vcpkg, builds the recovery compiler
with MSVC, and runs the platform-neutral 32-test profile. A distinct Windows
job now proves the experimental static SDK clean-install consumer path. Neither
Windows job attempts the Ubuntu-only interoperability or Python-reference checks.

```powershell
vcpkg install nlohmann-json:x64-windows openssl:x64-windows

cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release `
  "-DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  -DBUILD_TESTS=OFF -DBUILD_PYTHON_BINDINGS=OFF -DBUILD_CLI_TOOLS=OFF `
  -DBUILD_COMPILER_EXECUTABLE=OFF -DBUILD_EXPERIMENTAL_COMPONENTS=OFF `
  -DBUILD_REPL=OFF -DBUILD_CORE_SMOKE_TESTS=ON `
  -DBUILD_RECOVERY_NATIVE_SDK=OFF `
  -DBUILD_RECOVERY_INTEROP_SMOKE_TESTS=OFF `
  -DBUILD_OPENQASM_REFERENCE_CHECKS=OFF

cmake --build compiler/build --config Release --parallel 2
ctest --test-dir compiler/build -C Release --output-on-failure
```

## Reproduce the macOS Clang smoke profile

The macOS job has an independent Homebrew and Clang setup. It proves the same
32-test platform-neutral compiler/CLI/C-ABI smoke profile as Windows. A separate
macOS job proves the experimental static-SDK clean-install external-consumer
path; neither job validates language-interoperability fixtures or package
distribution.

```bash
brew update
brew install cmake nlohmann-json openssl@3

cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release \
  "-DCMAKE_PREFIX_PATH=$(brew --prefix nlohmann-json);$(brew --prefix openssl@3)" \
  -DBUILD_TESTS=OFF -DBUILD_PYTHON_BINDINGS=OFF -DBUILD_CLI_TOOLS=OFF \
  -DBUILD_COMPILER_EXECUTABLE=OFF -DBUILD_EXPERIMENTAL_COMPONENTS=OFF \
  -DBUILD_REPL=OFF -DBUILD_CORE_SMOKE_TESTS=ON \
  -DBUILD_RECOVERY_NATIVE_SDK=OFF \
  -DBUILD_RECOVERY_INTEROP_SMOKE_TESTS=OFF \
  -DBUILD_OPENQASM_REFERENCE_CHECKS=OFF

cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
```

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

The separate source-only Alpha Rust wrapper is tested only through the Ubuntu
full recovery profile. It links the test/build C ABI shared library through
`SYNQ_FFI_DIR` and is documented in
[`EXPERIMENTAL_RUST_WRAPPER.md`](./EXPERIMENTAL_RUST_WRAPPER.md). It does not
expand the installed static SDK into a Rust package or a portable shared-library
delivery mechanism.

The fixed Ubuntu 22.04 job is described in
[`SECOND_CLEAN_INSTALL_ENVIRONMENT.md`](./SECOND_CLEAN_INSTALL_ENVIRONMENT.md).
Together with the Windows MSVC and macOS Clang jobs, it validates the same
experimental static path in three CI operating-system families. It does not
broaden the support boundary to shared libraries, registry packages, stable ABI
compatibility, or general cross-platform distribution.

## Reporting an environment problem

When the recovery profile fails, open a GitHub issue with the host OS/release,
CMake and compiler versions, complete configuration command, first failing CTest
output, and whether the failure occurs before or after the clean-install check.
Do not report untested platforms as regressions until a maintainer reproduces or
adds them to the verified matrix.
