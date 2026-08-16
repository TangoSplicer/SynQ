# Second Clean-Install Environment Conformance

**Status:** Fixed-environment conformance job published for validation. The
Ubuntu 22.04 result is not a supported-platform claim until its GitHub Actions
job completes successfully.

## Purpose

The existing Compiler Core profile exercises the experimental static native SDK
consumer as one test in an `ubuntu-latest` recovery build. This record defines a
second, deliberately minimal environment that independently verifies the
producer/consumer installation boundary without installing the full Rust, Lisp,
Clojure, Mercury, or OpenQASM test stack.

## Fixed environment and contract

The `clean-install-sdk-ubuntu-22` job in
[`compiler-core.yml`](../.github/workflows/compiler-core.yml) uses GitHub-hosted
`ubuntu-22.04`, installs only `cmake`, `g++`, `nlohmann-json3-dev`, and
`libssl-dev`, builds `synq_lib` with smoke targets and the CLI disabled, installs
to a fresh temporary prefix, then configures, builds, and executes the existing
external C-header consumer using only `CMAKE_PREFIX_PATH`.

The job validates the experimental static archive, installed public header,
`SynQ::synq_lib` CMake package discovery, dependency discovery, C-header
consumer compilation, C++ static-library linkage, and opaque ABI parse/export/
cleanup call path. It does not validate dynamic-library distribution, a package
registry, signing, reproducibility, macOS, Windows, a stable ABI, or general
language support.

## Local mirror result

The job command sequence was run locally in a newly created producer build,
temporary install prefix, and separate external-consumer build directory before
publication. This proves the command sequence under the local Ubuntu 24.04
review environment only. The eventual Ubuntu 22.04 Actions result is required
before this record can add that platform to the tested-environment table.
