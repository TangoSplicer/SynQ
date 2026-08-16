# Second Clean-Install Environment Conformance

**Status:** Remotely validated fixed-environment conformance job in [Compiler
Core #50](https://github.com/TangoSplicer/SynQ/actions/runs/31956231719) for
revision `addad26`. This is a second Ubuntu evidence point, not a cross-platform
support claim.

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
publication. [Compiler Core #50](https://github.com/TangoSplicer/SynQ/actions/runs/31956231719)
then completed both its established 27-check recovery profile and the separate
Ubuntu 22.04 static-SDK producer/install/external-consumer job successfully.
This adds Ubuntu 22.04 static-SDK conformance evidence only; it does not validate
macOS, Windows, other Linux distributions, alternate compilers, dynamic linking,
registry delivery, signing, or ABI stability.
