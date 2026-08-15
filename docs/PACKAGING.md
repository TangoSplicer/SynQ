# Recovery-Profile CLI Packaging

**Status:** Locally validated experimental install/archive path; no remote
validation claim is made in this record.
**Last reviewed:** 15 August 2026

## Supported artifact

The CMake recovery profile builds the versioned experimental executable
`synqc 0.1.0-experimental`. It is the only installed program surface introduced
by this packaging path. The package includes the CLI and its bounded-evaluation/
simulation documentation; it does not install, freeze, or distribute a C ABI
SDK, language-specific wrapper, quantum provider connector, or full SynQ runtime.

## Build, test, install, and archive

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
cmake --install compiler/build --prefix "$PWD/compiler/stage"
cpack --config compiler/build/CPackConfig.cmake -B compiler/package
```

The default build enables `SYNQ_ENABLE_BUILD_HARDENING`. On supported GNU/Clang
builds this applies warning flags to the recovery library/CLI and ELF RELRO/NOW
linker flags to `synqc` on non-Apple Unix systems. It is not a completed security
audit, a memory-safety proof, or cross-platform hardening certification.

## Release boundary

> The `0.1.0-experimental` label is a compatibility warning, not a promise of a
> stable CLI, ABI, language specification, distribution channel, or runtime.

Every candidate must pass the documented compiler checks and be treated as an
experimental recovery-profile artifact until versioning, compatibility policy,
release signing, supply-chain review, and broader platform testing are separately
implemented.

## Local delivery evidence

The documented Release-profile path built the hardened compiler profile, passed
the full local **26/26** CTest suite, installed `bin/synqc` plus the three
recovery-profile documents into an isolated staging prefix, and generated
`synq-recovery-cli-0.1.0-experimental-Linux.zip` through CPack. This is local
evidence pending publication and compiler-core CI. It does not demonstrate
cross-platform archives, signing, reproducible builds, provenance attestation,
or stable binary compatibility.
