# SynQ Project Status

**Review date:** 20 August 2026
**Status:** Experimental recovery-profile evidence ledger. This document does
not declare SynQ feature-complete, production-ready, or fully operational.

> **Evidence rule:** a capability is described as verified only when the current
> code path, focused tests, and a cited validation result support it. Checked-in
> source, older roadmap text, or an optional target alone is not sufficient.

## Current verified baseline

[Compiler Core platform-matrix run #32485055898](https://github.com/TangoSplicer/SynQ/actions/runs/32485055898)
passed all seven independent jobs for revision `8d939d1`: the ordinary Linux
recovery profile passed **49/49** CTest checks; Windows MSVC and macOS Clang each
passed **35/35** platform-neutral CTest checks; Ubuntu 22.04, Windows MSVC, and
macOS Clang each passed the experimental static-SDK consumer; and an additive
Linux/Clang ASan/UBSan profile passed **34/34** isolated core tests. The exact
tested-environment boundary is recorded in [`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md).

| Area | Verified status | Evidence and boundary |
| --- | --- | --- |
| Compiler core | **Remotely validated, independently scoped profiles.** | The Ubuntu Linux profile builds `libsynq_lib.a`, `synqc`, test-only `libsynq_ffi.so`, and passes 49 CTests. Separate Windows MSVC and macOS Clang profiles each pass 35 platform-neutral compiler/CLI/C-ABI CTests. An additive Linux/Clang 34-test ASan/UBSan profile covers only its isolated core configuration. Optional historical targets remain outside these claims. |
| `synqc` CLI | **Remotely validated experimental workflow.** | Supports validation, bounded AST/strict-Hybrid OpenQASM source output, immutable/state evaluation, U5/U6 `--eval-runtime`, and bounded local probabilities. It is not a general executor, provider client, REPL, package manager, or stable CLI. |
| Local classical evaluation profile | **Draft contract only.** | [`LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md`](./LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md) maps the already implemented, mutually exclusive constants/state/runtime evaluator modes to a shared value/scope/budget vocabulary and profile-level acceptance gaps. It adds no execution semantics, runtime-completion claim, stable diagnostic contract, or conformance closure. |
| Local evaluator composition evidence | **Remotely validated narrow profile-separation fixture in [Compiler Core #32485055898](https://github.com/TangoSplicer/SynQ/actions/runs/32485055898).** | `synq_local_classical_evaluation_composition_smoke` checks shared immutable results across existing modes and atomic rejection of state/callable, callable-only, mixed state/callable, and quantum crossings. It verifies existing evaluator separation only; it does not merge modes, add a callable/state composition, close the profile matrix, or establish a complete runtime. |
| Published `synqc` experimental pre-release | **Published from the remotely validated tag-aware candidate path in [synqc Release Candidate #32482750179](https://github.com/TangoSplicer/SynQ/actions/runs/32482750179).** | The annotated `v0.1.0-experimental` tag resolves to the exact Compiler Core-tested commit. Linux X64, macOS ARM64, and Windows X64 jobs built exact CLI-only archives, JSON/SHA-256 manifests, content allowlists, clean extraction, `--version`/`--help`/SynQ validation smoke, and GitHub provenance attestations. The [pre-release](https://github.com/TangoSplicer/SynQ/releases/tag/v0.1.0-experimental) uploads only those three archives, three manifests, and three checksum files. The archives are unsigned; the macOS archive is not notarized; no installer, package-manager package, stable ABI/runtime, hardware-service, or general platform-support claim follows. |
| Typed source model | **Remotely validated bounded subset.** | Supports immutable declarations, Alpha top-level mutable cells, typed gates, named measurement provenance, Alpha expressions, zero-parameter/U3 parameterized kernels, one U4 feedback pair, U5 one-formal local functions, and U6 two-formal same-type local functions. No general returns, lexical scopes, recursion, nested routine calls, general branch execution, target-side state lowering, or general runtime semantics exist. |
| Bounded Boolean declaration evaluation | **Remotely validated Alpha compile-time subset.** | `synqc --eval-constants` deterministically evaluates immutable top-level Boolean literal/reference/`not`/`and`/`or` trees after static resolution, with default expression-depth `16`, request-wide operation budget `128`, and no short-circuit runtime meaning. Depth and operation exhaustion yield structured diagnostics. No assignment, state mutation, loop/branch execution, target-side expression execution, measurement-result value, or simulation execution is claimed. |
| Bounded mutable classical state | **Remotely validated Alpha local-only subset.** | Feature-gated top-level `var` and whole-cell `set` use fixed Boolean/Integer/String static types, prior-only resolution, source-order snapshot evaluation, defaults of 64 cells/128 state transitions/16 expression depth/128 operations, and final declaration/write provenance through `synqc --eval-state`. Parser, typed AST/IR, resolver, evaluator, CLI, semantic inspection, and rejection fixtures passed. Strict Hybrid export and bounded quantum simulation reject state nodes; no branches, loops, target storage, measurement values, ABI state execution, or hardware execution is claimed. |
| Alpha semantic environment | **Remotely validated read-only inspection subset.** | `synqc --inspect-semantics` renders resolved top-level classical binding metadata: kind, static type, source line, and earlier-binding dependencies. It does not evaluate general source, produce runtime values, sample measurements, or introduce nested scopes. |
| Named registers | **Remotely validated Alpha subset with bounded local simulation.** | Earlier declared `name[index]` operands resolve, strict-Hybrid export preserves declared registers, and the simulator maps explicit register declarations into a declaration-order bounded state vector while retaining source provenance. No lifetime, aliasing, deallocation, dynamic allocation, or hardware mapping semantics exist. |
| Bounded if export | **Remotely validated strict source-lowering subset in [Compiler Core #32188899985](https://github.com/TangoSplicer/SynQ/actions/runs/32188899985).** | One literal `if`, `if not true/false` through compile-time folding, one `if` controlled by an earlier top-level Boolean-literal declaration, or `if not <that declaration>`, with one supported typed gate body lowers to OpenQASM 3. Nested negation, binary expressions, aliases, measurement results, `while`, measurement bodies, and execution remain rejected. |
| Local simulation | **Remotely validated bounded probability model.** | One or more explicit declared registers, limited gates, opt-in combined resource limits, and deterministic probability output with source-register provenance. U4 additionally enumerates at most one terminal named-measurement/direct-`x` branch pair without sampling a host value. No general collapse API, noise, general control execution, provider, hardware, lifetime, or dynamic allocation behavior. |
| C ABI and static SDK | **Remotely validated experimental static path on Ubuntu, Windows, and macOS.** | Ubuntu 22.04, Windows MSVC, and macOS Clang clean-install jobs each prove static library/header/CMake package discovery and the external C consumer. No frozen ABI, shared-library delivery, registry, or general cross-platform SDK guarantee is made. |
| Bounded callable-kernel export | **Remotely validated strict source-lowering subset in [Compiler Core #32191872033](https://github.com/TangoSplicer/SynQ/actions/runs/32191872033).** | One earlier zero-parameter `kernel` with exactly one non-parameterized default-register gate body, plus one later `call`, expands into the typed gate during strict Hybrid OpenQASM source generation. No function bodies, arguments, returns, named-register operands, recursion, nested calls, control bodies, simulation, ABI execution, or runtime dispatch are claimed. |
| Bounded parameterized quantum routines | **Remotely validated U3 Alpha strict source-lowering subset in [Compiler Core #32247154982](https://github.com/TangoSplicer/SynQ/actions/runs/32247154982).** | The `parameterized-quantum-routines` feature accepts exactly one literal-angle plus one qubit formal with `rx`/`ry`/`rz`/`p`, one qubit formal with `h`/`x`/`y`/`z`, or two qubit formals with `cx`. Resolver checks earlier definition, ordered actual count/kind, declared in-range operands, and duplicate two-qubit aliases. Strict Hybrid export expands at most 32 declarations and 128 calls into source-order gates; simulator and C ABI explicitly reject U3 nodes. No routine execution, target-side `def`, capture, allocation, recursion, nested calls, runtime angle, hardware, or provider behavior is claimed. |
| Bounded measurement feedback | **Remotely validated U4 Alpha hybrid subset in [Compiler Core #32250265354](https://github.com/TangoSplicer/SynQ/actions/runs/32250265354).** | The `measurement-feedback` feature accepts one top-level named measurement immediately followed by one direct `if <same-result> then quantum x register[index]`. Resolver retains typed result provenance and rejects non-direct, reused, non-adjacent, invalid, and post-feedback forms. Strict Hybrid export emits one exporter-owned scalar bit, one measurement assignment, and one conditional `x`; local simulation enumerates the two measurement branches and returns weighted final probabilities without sampling. The C ABI explicitly rejects U4 nodes. No `else`, loop, generic control, general collapse API, target-side user state, routine feedback, provider, or hardware behavior is claimed. |
| Bounded classical callable runtime | **Remotely validated U5 Alpha local subset in [Compiler Core #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516).** | The `classical-callable-execution` feature accepts one earlier `fn name(value: Integer|Boolean|String) ->` parameter-only body and one immutable `let result = name(actual)`. `--eval-runtime` uses one explicit local frame, checked `int64` arithmetic, exact static types, defaults of 32 declarations/128 invocations/depth 1/depth 16/128 operations, and atomic failure. Strict Hybrid export, bounded quantum simulation, and ABI v1 explicitly reject U5 nodes. No capture, mutable local state, multi-argument calls, returns, recursion, nested calls, target execution, provider, or hardware behavior is claimed. |
| Bounded binary classical callable runtime | **Remotely validated U6 Alpha local subset in [Compiler Core #32270327206](https://github.com/TangoSplicer/SynQ/actions/runs/32270327206).** | The jointly gated `classical-callable-execution` and `multi-formal-classical-callables` features accept one earlier `fn name(left: Integer|Boolean, right: same-type) ->` documented binary body and one immutable ordered two-actual invocation. `--eval-runtime` uses a two-binding non-capturing frame, exact static types, checked `int64` arithmetic, deterministic Boolean operators, U5’s declaration/invocation/depth/expression/operation defaults, and atomic failure. Strict Hybrid export, bounded quantum simulation, and ABI v1 explicitly reject U6 nodes. No arbitrary arity, mixed types, String binary body, capture, mutable local state, returns, recursion, nested calls, target execution, provider, or hardware behavior is claimed. |
| Deterministic parser/runtime replay | **Remotely validated fixed-corpus replay target in [Compiler Core #32374149046](https://github.com/TangoSplicer/SynQ/actions/runs/32374149046).** | `synq_deterministic_replay_smoke` replays five checked-in NUL-free SynQ sources through in-memory parsing, Hybrid IR lowering, resolution, and bounded evaluator calls. Each base source and eight fixed single-byte mutations run twice and compare terminal-stage/diagnostic/value digests. It is not continuous fuzzing, libFuzzer, property coverage, a complete corpus, or a security claim. |
| Zero-cost sanitizer core profile | **Remotely validated additive Linux/Clang test profile in [Compiler Core #32403435066](https://github.com/TangoSplicer/SynQ/actions/runs/32403435066).** | An opt-in `SYNQ_ENABLE_SANITIZERS=ON` build applies ASan/UBSan and passed 33/33 isolated core CTests with leak detection and halt-on-error settings, including deterministic replay. It does not establish sanitizer coverage for ordinary Linux recovery, Windows, macOS, SDK, interop, reference-parser, artifact, continuous fuzzing, audit, or production-security paths. |
| Interoperability proofs | **Remotely exercised on the Ubuntu full profile.** | C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend fixtures use the opaque C ABI. The Windows profile intentionally excludes those toolchain-specific fixtures. They are not released wrappers or language packages. |
| Alpha Rust wrapper | **Remotely exercised source-only adapter on the Ubuntu full profile.** | The dependency-free `synq-alpha` Cargo package owns the opaque program handle through RAII and exposes parse, OpenQASM 3 export, and ABI-identifier helpers. Three Cargo conformance tests pass through CMake against the test/build C ABI shared library. It is not published to a registry, bundled as a shared-library distribution, or a stable Rust API. |
| Governance and maintenance | **Factual baseline published.** | `CHANGELOG.md`, contribution guidance, a security-reporting route, issue templates, support-environment guide, and ABI/distribution policy exist. They do not promise review times, support SLA, signing, or package availability. |

## Explicit non-claims

SynQ currently makes **no verified claim** of a stable language or ABI release,
general runtime, complete type system, general classical runtime/evaluator, general callable
execution, IDE, provider integration, quantum-hardware access, noise model,
measurement sampling, cross-platform distribution, performance target,
enterprise support, commercial service, or complete project-wide test coverage.

The repository frontend/backend directories and historical documents are not
evidence of a deployed product or compiler-service integration unless a document
states a reproducible, current validation path.

## Ordered remaining work

1. Define wider callable and control contracts only after arbitrary parameter
   shapes, scopes, returns, resource effects, recursion rules, and execution
   boundaries have separate written contracts and tests.
2. Define any expansion beyond U4 only after multiple-result lifetime, `else`,
   general control, and target/runtime boundaries have separate contracts and tests.
3. Expand language-specific wrappers or package delivery only after ownership,
   compatibility, environment, and conformance commitments are explicit.

## Related records

The detailed evidence trail is in
[`REVIEW_EVIDENCE_2026-08-13.md`](./REVIEW_EVIDENCE_2026-08-13.md). Current
operational gates are in [`OPERATIONAL_READINESS.md`](./OPERATIONAL_READINESS.md),
and the dependency-ordered assessment is in
[`OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md`](./OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md).
The current limits, deterministic replay contract, and differentiation/usage
audit are in [`KNOWN_LIMITS_v0.1.0.md`](./KNOWN_LIMITS_v0.1.0.md),
[`DETERMINISTIC_PARSER_RUNTIME_REPLAY.md`](./DETERMINISTIC_PARSER_RUNTIME_REPLAY.md),
and [`UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md`](./UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md).
