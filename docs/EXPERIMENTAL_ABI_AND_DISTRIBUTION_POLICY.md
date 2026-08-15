# SynQ Experimental ABI and Distribution Policy

**Policy status:** Active experimental policy for the recovery-profile C ABI and
CLI delivery path. It is **not** a stability promise, supported-SDK policy, or
production release policy.  
**Effective from:** 15 August 2026  
**Applies to:** `compiler/include/synq/synq_ffi.h`, `synq_lib`, the test-only
`synq_ffi_shared` target, and the `synqc 0.1.0-experimental` CLI delivery path.

## 1. Policy purpose

SynQ needs a public boundary before it can responsibly grow host-language
wrappers or package artifacts. This policy fixes the meaning of the current
**experimental** boundary: what a consumer may rely on while using a particular
source revision, what the project will document when that boundary changes, and
what the existing install/archive path does and does not distribute.

> **Experimental means inspectable, not stable.** A v1 ABI identifier allows
> consumers to reject an incompatible library at runtime; it does not create a
> promise of backward compatibility across commits, releases, platforms, or
> distribution channels.

This policy does not widen the language, create a new shared-library product, or
declare any language-specific package available. The authoritative C surface is
the checked-in header, not this explanatory document.[1]

## 2. Current contract identity

| Identifier | Current value | Meaning | Boundary |
| --- | --- | --- | --- |
| C ABI major | `SYNQ_ABI_VERSION` = `1` | The integer returned by `synq_abi_version()`. | It identifies the current experimental header shape; it is not a frozen ABI guarantee.[1] |
| C ABI string | `synq-c-abi/1` | Static identifier returned by `synq_version()`. | It is library-owned, NUL-terminated, and must not be freed or modified by callers.[1] |
| CLI version | `0.1.0-experimental` | Version string printed by `synqc --version`. | It labels a bounded experimental command, not the C ABI’s stability level or a general language release.[2] |
| Evidence baseline | Compiler Core #45, 26/26 checks | The recovery compiler profile, including CLI and interop smoke checks, passed on GitHub Actions for implementation revision `202ebaf`.[3] | It does not test binary artifacts distributed to arbitrary systems. |

The CLI version and ABI major are **independent version domains**. A CLI version
change does not by itself change the ABI major; an ABI-major change does not by
itself make the CLI stable. Any release notes must name both values when both
interfaces are relevant.

## 3. Experimental C ABI v1 surface

ABI v1 contains only the symbols declared by
[`synq_ffi.h`](../compiler/include/synq/synq_ffi.h). The public function names,
opaque-handle type, and status values are listed below for policy review; the
header remains authoritative if text ever conflicts.

| Surface | v1 behavior | Caller responsibility | Explicit non-claim |
| --- | --- | --- | --- |
| `synq_abi_version()` | Returns ABI major `1`. | Check it before relying on a particular v1 header contract. | No future compatibility promise is inferred from the value. |
| `synq_version()` | Returns a static ABI identifier string. | Do not modify or free the pointer. | It is not a semantic-versioned compiler or package version. |
| `synq_parse_file()` | Parses one non-empty UTF-8 source-file path through the bounded recovery parser. | Release a successful opaque handle with `synq_program_free()`; release any returned diagnostic with `synq_string_free()`. | It does not parse a complete SynQ language or guarantee path/encoding portability beyond the documented recovery profile.[4] |
| `synq_parse_source()` | Parses one NUL-terminated in-memory source string without retaining caller storage. | Do not pass embedded-NUL source; use the same ownership rules as file parsing. | It is not a length-aware buffer, streaming, or editor API.[4] |
| `synq_export_openqasm3()` | Emits only the bounded supported OpenQASM 3 source subset from an opaque parsed handle. | Release emitted text and diagnostic strings with `synq_string_free()`. | It is not execution, circuit-equivalence proof, provider submission, or hardware access.[4] |
| `synq_string_free()` | Releases a library-allocated output or diagnostic string; accepts `NULL`. | Call exactly once for each non-`NULL` returned library string. | Callers may not use another allocator. |
| `synq_program_free()` | Releases a successfully returned opaque program handle; accepts `NULL`. | Call exactly once; do not copy, serialize, transfer across processes, or use after release. | The handle is not an AST/IR API or a stable serialized representation.[4] |

The v1 `synq_status` enum contains only `OK`, `INVALID_ARGUMENT`,
`PARSE_ERROR`, `EXPORT_ERROR`, and `INTERNAL_ERROR` as declared in the header.[1]
Diagnostic prose and embedded diagnostic-code text are useful current evidence,
but they are **not separately versioned protocol guarantees** under this
experimental policy.

## 4. Compatibility and change rules

The project will make incompatibility visible instead of silently changing a
consumer’s assumptions. Because ABI v1 remains experimental, a change may break
compatibility; when it does, the project must perform the actions in the table
before presenting the change as a new ABI revision.

| Change category | Examples | Required project action before publication |
| --- | --- | --- |
| **ABI-breaking** | Changing an exported name, C signature, enum value, ownership/lifetime rule, opaque-handle rule, calling convention, or removing a declared symbol. | Increment `SYNQ_ABI_VERSION`; update `synq_version()`; revise this policy, `C_ABI.md`, and all supported consumer fixtures; add migration notes; run the recovery profile and publish CI evidence. |
| **Potentially behavior-breaking** | Altering accepted bounded source, error classification, or exact exported OpenQASM for an existing accepted fixture while the header stays unchanged. | Keep the ABI major unless the header contract changes, but document the semantic break in release notes and source status; update positive/negative fixtures and all affected consumer expectations. |
| **Compatible extension** | Adding a new optional function or capability while preserving all existing declarations, enum values, ownership, and documented bounded behavior. | Preserve existing v1 fixtures, document the feature/support boundary, and add C plus applicable consumer tests before any public support claim. |
| **Internal-only change** | Refactoring C++ AST/HIR implementation without changing public-header behavior. | Keep the public ABI unchanged and retain the existing opaque-ABI smoke coverage. |

No application may assume that ABI v1 artifacts built from different commits are
interchangeable merely because both return `1`. Consumers that need reproducible
behavior should record the SynQ commit, compiler/toolchain, build options, and
the exact header used to compile them.

## 5. Current distribution policy

### 5.1 Supported recovery delivery surface

The only installable product currently configured by the recovery packaging path
is the `synqc` executable. CMake installs `bin/synqc` and three bounded-workflow
documents; CPack creates a ZIP named from `synq-recovery-cli` with version
`0.1.0-experimental`.[2] The documented install and CPack commands have been
performed locally; their operations are not remote package-release evidence.[5]

| Artifact or route | Current policy | Reasoning and limitation |
| --- | --- | --- |
| `synqc` CLI and bundled documents | **Experimental local install/archive path.** | This is the configured install/ZIP delivery surface. It has no signing, provenance attestation, cross-platform support matrix, or support SLA.[2] [5] |
| `synq_lib` static library and CMake package | **Locally validated experimental native SDK path.** | The recovery packaging configuration installs the static library, public header, and `SynQ::synq_lib` CMake package; an isolated C/C++ consumer passed locally. Remote CI evidence remains pending, and no stable SDK or platform support claim is made.[6] |
| `synq_ffi_shared` / `libsynq_ffi.so` | **Test-only build-directory artifact.** | It exists to run Common Lisp, Clojure/JNA, and Mercury smoke consumers; it is not a distributed native library.[6] |
| Rust, Common Lisp, Clojure, Mercury integrations | **Remotely tested consumer proofs only.** | No crate, CFFI/ASDF system, Clojure/JVM library, Mercury package, or installer is published by this policy.[4] [6] |
| Registries and operating-system packages | **Not published.** | No crates.io, Quicklisp, Maven, Mercury, Homebrew, apt, Chocolatey, or equivalent package claim is made. |

### 5.2 Consumer acquisition rule

The current supported evaluation route for the native ABI is a source checkout
and the recovery CMake build profile used by the existing smoke fixtures. A
consumer may inspect and compile against the public header from that same source
revision, but must not treat build-directory library locations or test helper
targets as an installation contract.[1] [6]

The current recovery path now has explicit header/library installation, CMake
package metadata, and a local clean-prefix conformance test. Before the project
claims a supported SDK or language-specific wrapper, it still needs remote CI
evidence, a platform support statement, artifact-version policy, compatibility
matrix, signing/provenance policy, and clean-environment coverage beyond the
current Linux recovery environment.[6]

## 6. Validation and publication discipline

Every experimental ABI or delivery-path change must retain the existing source
and ownership tests. The minimum evidence sequence is deliberately compatible
with zero-cost local tooling and GitHub Actions:

1. Build the recovery profile using the documented CMake command.
2. Run `ctest --test-dir <build> --output-on-failure`; the current evidence
   baseline is 26 focused checks.[3]
3. Exercise `synqc --version` and the documented bounded CLI smoke path.
4. For an intended CLI archive, run the documented local install and CPack
   commands, then inspect that the archive reflects only the configured CLI and
   documentation surface.[5]
5. Publish the commit and record the resulting Compiler Core URL, exact commit,
   test count, and any local-only operations separately.

A failed consumer, package, or compatibility check blocks a support claim for
that affected route. It does not justify suppressing or broadening an error
through the raw ABI.

## 7. Support, security, and maintenance limits

SynQ is a solo-maintained experimental project with no commercial support SLA,
no committed response time, and no published dedicated security-reporting
channel under this policy. The repository should not be represented as providing
enterprise support, a security certification, vulnerability response guarantees,
or operational service availability.

Issues and Discussions may be used for normal public feedback. A dedicated
security disclosure process, known-issues ledger, release signing/provenance
policy, and maintained platform matrix are prerequisites for stronger
distribution or security claims.

## 8. Policy milestones

| Milestone | Status after this policy | Evidence still required |
| --- | --- | --- |
| Experimental ABI contract policy | **Published by this document.** | Keep it synchronized with every public-header change. |
| ABI v1 consumer proof | **Remotely validated through focused C/Rust/Common Lisp/Clojure/Mercury tests.** | Add a conformance matrix only when actual distribution begins.[3] [4] |
| Installed native SDK | **Locally validated experimental static C ABI path.** | Remote CI evidence, supported-platform matrix, signing/provenance, artifact version policy, and compatibility coverage. |
| Language-specific package | **Not available.** | One maintained package per ecosystem with native ownership/error tests and a version policy. |
| Stable ABI or release channel | **Not available.** | Compatibility commitment, migration policy, signing/provenance, release process, and broader test/platform evidence. |

## References

[1]: ../compiler/include/synq/synq_ffi.h "SynQ public experimental C ABI header"

[2]: ../compiler/CMakeLists.txt "SynQ recovery CMake configuration"

[3]: https://github.com/TangoSplicer/SynQ/actions/runs/31887461976 "SynQ Compiler Core #45 — successful 26-check recovery profile"

[4]: ./C_ABI.md "SynQ C ABI Foundation"

[5]: ./PACKAGING.md "Recovery-Profile CLI Packaging"

[6]: ../compiler/CMakeLists.txt "SynQ test-only shared ABI and language-consumer wiring"

[7]: ./EXPERIMENTAL_NATIVE_SDK.md "Experimental Native SDK design and local evidence"
