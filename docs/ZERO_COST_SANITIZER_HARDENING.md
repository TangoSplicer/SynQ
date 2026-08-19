# SynQ Zero-Cost Sanitizer Hardening Contract

**Status:** Locally validated implementation; remote verification pending. This
document does not claim that a sanitizer build is remotely verified, or that a
fuzz campaign, security audit, or runtime reliability closure has been completed.

## Objective

Add one **additive Linux-only sanitizer core profile** to the free Compiler Core
workflow. The profile must build selected recovery targets with AddressSanitizer
(ASan) and UndefinedBehaviorSanitizer (UBSan), run a bounded CTest subset, and
fail the job on sanitizer diagnostics. It is reliability evidence for the tested
core profile; it is not a production hardening mode or a security certification.

The existing six verified Compiler Core jobs remain unchanged. A sanitizer job is
additional evidence, not a replacement for Linux full recovery, Windows/macOS
platform-neutral smoke, or the three static-SDK consumer jobs.

## Proposed profile

| Property | Contract |
| --- | --- |
| Profile identifier | `sanitizer-core-v0.1.0` |
| Runner | GitHub-hosted Ubuntu Linux, using Clang installed from the free native package path. |
| Instrumentation | `-fsanitize=address,undefined`, frame pointers, debug information, and non-recovering undefined-behavior diagnostics. |
| Runtime settings | Leak detection and halt-on-error for ASan; halt-on-error and stack traces for UBSan. |
| CMake activation | A new option disabled by default, so developer and distribution builds retain their current flags unless they explicitly opt in. |
| Test scope | The local compiler core, recovery CLI, parser/resolver/evaluator/exporter/simulator/C-ABI smoke tests that can be linked and executed in the isolated sanitizer profile. |
| Exclusions | Mercury, Common Lisp, Clojure, direct Rust, installed static-SDK consumer, artifact delivery, and reference-parser paths may remain in the existing profiles and are not silently relabelled as sanitizer evidence. |

ASan is a compiler instrumentation/runtime tool for classes of memory errors;
UBSan instruments selected undefined behavior checks. Both are testing tools, and
their runtimes are not a production-security substitute.[1] [2]

LLVM documents that libFuzzer is an in-process coverage-guided engine whose
target must tolerate malformed input and remain deterministic; that separate
fuzz-target/corpus/reproduction design is intentionally deferred rather than
misrepresented by the sanitizer profile.[3]

## Safety and determinism rules

The profile must use no provider account, hardware credential, AI API, paid
service, or private secret. It will execute only the checked-in deterministic
CTest profile and write no corpus or report back into the repository. A failure
must preserve the ordinary CTest output and sanitizer report for diagnosis.

No sanitizer suppression, ignorelist, or recovery setting may be added merely to
make the profile pass. A genuine report must be fixed, documented as an external
dependency issue with a narrow temporary boundary, or cause the profile to fail.

## Acceptance evidence

The profile is **locally validated** only when a clean Clang configure, build,
and bounded CTest run passes with sanitizer diagnostics enabled. It is **remotely
validated** only when the additive workflow job and every existing Compiler Core
job pass for the same implementation revision. The public record must state the
sanitizer job’s exact CTest count, operating system, compiler, flags, runtime
settings, and excluded paths.

### Local evidence — 19 August 2026

An isolated clean Clang `RelWithDebInfo` configuration with
`SYNQ_ENABLE_SANITIZERS=ON` built and passed **32/32** platform-neutral recovery
CTests under `ASAN_OPTIONS=detect_leaks=1:halt_on_error=1` and
`UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1`. The ordinary recovery profile
was then rebuilt and passed **46/46** CTests without sanitizer instrumentation.
Finally, configuring the sanitizer option with GNU C/C++ compilers failed with
the documented `SYNQ_ENABLE_SANITIZERS requires Clang` message. These are local
checks only; the additive workflow has not yet supplied remote evidence.

## Explicit non-claims

This increment does not add fuzzing, property testing, deterministic replay,
coverage thresholds, exhaustive memory-safety proof, threat modeling, security
audit, performance guarantee, stable ABI, general runtime, or hardware-service
claim. libFuzzer is a possible later free toolchain option, but a bounded
coverage-guided campaign needs its own target, corpus, time budget, artifact
handling, and reproduction contract.[3]

## References

[1]: https://clang.llvm.org/docs/AddressSanitizer.html "Clang AddressSanitizer documentation"
[2]: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html "Clang UndefinedBehaviorSanitizer documentation"
[3]: https://llvm.org/docs/LibFuzzer.html "LLVM libFuzzer documentation"
