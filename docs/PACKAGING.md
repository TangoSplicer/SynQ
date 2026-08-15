# Recovery-Profile CLI Packaging

**Status:** Experimental install/archive path with a remotely validated
compiler build-and-test baseline. The installation and archive commands remain
locally validated operations.
**Last reviewed:** 15 August 2026

## Supported artifact

The CMake recovery profile builds the versioned experimental executable
`synqc 0.1.0-experimental`. Its locally validated archive also includes the
experimental static C ABI SDK surface: `libsynq_lib.a`,
`include/synq/synq_ffi.h`, and a `SynQ::synq_lib` CMake package. This does not
freeze the ABI or distribute a language-specific wrapper, quantum provider
connector, full SynQ runtime, dynamic library, signed artifact, or stable SDK.

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

The active [Experimental ABI and Distribution Policy](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md)
separates this installable CLI path from the non-distributed C ABI and test-only
shared-library artifacts. In particular, this ZIP does not install `synq_lib`,
the public C header, or language-specific SDK packages.

Every candidate must pass the documented compiler checks and be treated as an
experimental recovery-profile artifact until versioning, compatibility policy,
release signing, supply-chain review, and broader platform testing are separately
implemented.

## Local delivery evidence

The documented Release-profile path passes the full local **27/27** CTest suite,
including clean-prefix `SynQ::synq_lib` consumer conformance. It installs
`bin/synqc`, the static library, public C header, CMake package metadata, and
the bounded-workflow/SDK documents into an isolated staging prefix. CPack
generates `synq-recovery-cli-0.1.0-experimental-Linux.zip` containing those
configured artifacts while excluding the test-only shared ABI library. [Compiler
Core #46](https://github.com/TangoSplicer/SynQ/actions/runs/31898767207) remotely
validated the 27-check SDK increment for revision `1de1484`; CPack archive
inspection remains local evidence. The evidence does not demonstrate
cross-platform archives, signing, reproducible builds, provenance attestation,
or stable binary compatibility.
