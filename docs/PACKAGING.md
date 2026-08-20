# synqc Packaging Status and Specification Index

**Status:** Current delivery boundary plus a link to the proposed major-platform
distribution specification. This is not a public binary-release announcement.

## Current delivery position

The recovery CMake profile builds `synqc 0.1.0-experimental`, installs it with
`cmake --install`, and configures a local ZIP-oriented CPack path. Compiler Core
currently uploads platform build artifacts but does not publish GitHub Release
assets, checksums, provenance attestations, signed binaries, installers, or
package-manager entries. The current remote source-build evidence is Compiler
Core [#32374149046](https://github.com/TangoSplicer/SynQ/actions/runs/32374149046):
47/47 ordinary Linux CTests, 33/33 Windows MSVC and macOS Clang CTests, three
experimental static-SDK consumer jobs, and 33/33 Linux/Clang sanitizer CTests.

The CMake delivery surface can include an experimental static SDK when
`BUILD_RECOVERY_NATIVE_SDK=ON`; that is a source/CMake integration path, not a
distributed CLI bundle or stable ABI. The test-only shared ABI and consumer
fixtures are never a release-library product. See
[`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md).

## Proposed path

[`SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md`](./SYNQC_PACKAGING_AND_DISTRIBUTION_SPEC_v0.1.0.md)
defines the proposed zero-cost path: CLI-only Linux/macOS archives and Windows
ZIPs, SHA-256 manifests, clean-extraction tests, immutable GitHub Release assets,
optional GitHub provenance attestations, explicit unsigned macOS/Windows limits,
and separate acceptance gates before publication.

> Until those gates are implemented and pass for the exact release commit, SynQ
> has a source-build/install path and CI artifacts—not verified public CLI
> packages.

## Current local commands

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
cmake --install compiler/build --prefix "$PWD/compiler/stage"
```

These commands are useful for local experimental evaluation. They do not create
a signed, notarized, attested, or publicly released package.
