# SynQ Testing and Evidence Guide

**Status:** Current guide for the bounded experimental recovery profile.
**Evidence baseline:** [Compiler Core #32496656921](https://github.com/TangoSplicer/SynQ/actions/runs/32496656921), revision `366cbd9`.

> Test counts are evidence snapshots, not a coverage percentage, a complete
> language claim, a security certification, or a promise that every tracked file
> participates in the recovery build.

## What this guide covers

The supported testing path is the CMake recovery compiler under `compiler/`.
It builds the C++17 implementation, `synqc`, the static core library, and named
CTest fixtures. The source programs being tested are SynQ programs; the compiler
implementation and native ABI consumers necessarily use host languages. See
[`docs/INTEROPERABILITY.md`](docs/INTEROPERABILITY.md) for the narrow role of
foreign-language conformance fixtures.

| Evidence layer | Current scope | Explicit exclusion |
| --- | --- | --- |
| Ordinary recovery profile | 50 Ubuntu CTests for parser, semantics, bounded evaluator, evaluator-mode composition and value diversity, exporter, simulator, CLI, C ABI, deterministic replay, candidate-package extraction, and named interop checks. | A project-wide test inventory or coverage figure. |
| Platform-neutral smoke | 36 CTests each on Windows MSVC and macOS Clang, including evaluator-mode composition, value diversity, and candidate-package extraction. | Toolchain-specific Lisp, Clojure, Mercury, reference-parser, and related Ubuntu-only checks. |
| Native SDK consumers | Clean-install static C SDK consumers on Ubuntu 22.04, Windows MSVC, and macOS Clang. | Shared-library distribution, a frozen ABI, or released language packages. |
| Sanitizer profile | 35 isolated Linux/Clang ASan/UBSan core CTests, including deterministic replay, evaluator-mode composition, and value diversity. | Continuous fuzzing, all-platform sanitizer coverage, external-parser coverage, or security certification. |

## Build and run the ordinary local recovery profile

Install the prerequisites described in
[`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md), then configure,
build, and run the exact local profile:

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
```

When the optional Mercury consumer is available locally, place its compiler on
`PATH` before configuring. Its absence must be recorded as an environment
difference; it must not be hidden by changing claims about the remote baseline.

## Run the narrow sanitizer profile

The sanitizer configuration is additive. It uses a separate build directory so
that ordinary evidence is preserved:

```bash
CC=clang CXX=clang++ cmake -S compiler -B compiler/sanitizer-build \
  -DSYNQ_ENABLE_SANITIZERS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build compiler/sanitizer-build --parallel 2
ctest --test-dir compiler/sanitizer-build --output-on-failure
```

This profile is deliberately narrower than the ordinary recovery profile. A
passing run confirms only the configured core tests; it does not prove that
every source file or dependency is sanitizer-clean.

## Test a supported SynQ program

Use a documented fixture or a bounded source program, rather than an invented
language surface. For example, the verified U6 callable shape can be exercised
through the explicit runtime mode:

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]
fn add(left: Integer, right: Integer) -> left + right

let total: Integer = add(20, 22)
```

```bash
./compiler/build/synqc example.synq --eval-runtime
```

The result is local bounded evaluation only. It is not a general function
runtime, OpenQASM lowering, ABI execution, provider integration, or hardware
submission. Read the associated contract before extending the example:
[`docs/BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md`](docs/BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md).

## Fixture ownership and failure reporting

The versioned fixture manifest maps selected source files to their CTest owners:
[`docs/CONFORMANCE_FIXTURE_MANIFEST_v0.1.0.md`](docs/CONFORMANCE_FIXTURE_MANIFEST_v0.1.0.md).
The U1–U6 matrix maps requirements to positive, negative, and boundary tests:
[`docs/CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md`](docs/CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md).

When reporting a failure, provide the SynQ source (or named fixture), exact
command, operating system/toolchain version, complete diagnostic, and the
expected bounded behavior. Do not infer a general language defect from a file
that is outside the supported recovery subset.

## Deterministic replay evidence

`synq_deterministic_replay_smoke` exercises five checked-in SynQ corpus sources
through parser, Hybrid IR lowering, name resolution, and all bounded evaluator
entry points. It replays each source unmodified and with eight fixed single-byte
mutation seeds, runs every input twice, and compares environment-independent
outcome digests. The target passed in the ordinary and sanitizer profiles of
Compiler Core #32374149046. It is a fixed, deterministic replay smoke target;
it does not claim continuous fuzzing, libFuzzer integration, property-test
coverage, or a security certification. See
[`docs/DETERMINISTIC_PARSER_RUNTIME_REPLAY.md`](docs/DETERMINISTIC_PARSER_RUNTIME_REPLAY.md)
and [`docs/KNOWN_LIMITS_v0.1.0.md`](docs/KNOWN_LIMITS_v0.1.0.md).

## Non-publishing candidate-package evidence

`synq_release_candidate_package_smoke` uses the exact built `synqc` binary to
make a CLI-only platform archive, write a JSON manifest and SHA-256 manifest,
enforce the archive allowlist, extract into a fresh directory, and run
`--version`, `--help`, and a verified SynQ `--validate` fixture. It is part of
the 48-test Linux and 34-test Windows/macOS ordinary profiles in Compiler Core
#32394772652. The separate
[synqc Release Candidate #32393670587](https://github.com/TangoSplicer/SynQ/actions/runs/32393670587)
passed three candidate jobs, each creating, extracting, attesting, and retaining
a candidate archive. Those retained artifacts are **not** public GitHub Release
assets, installers, signed binaries, or package-manager packages. See
[`docs/SYNQC_RELEASE_CANDIDATE_PIPELINE_DESIGN_v0.1.0.md`](docs/SYNQC_RELEASE_CANDIDATE_PIPELINE_DESIGN_v0.1.0.md).
