# SynQ Project Status

**Review date:** 19 August 2026
**Status:** Experimental recovery-profile evidence ledger. This document does
not declare SynQ feature-complete, production-ready, or fully operational.

> **Evidence rule:** a capability is described as verified only when the current
> code path, focused tests, and a cited validation result support it. Checked-in
> source, older roadmap text, or an optional target alone is not sufficient.

## Current verified baseline

 [Compiler Core platform-matrix run #32239066421](https://github.com/TangoSplicer/SynQ/actions/runs/32239066421)
passed all six independent jobs for revision `c9db036`: the Linux recovery
profile passed **37/37** CTest checks; Windows MSVC and macOS Clang each passed
**26/26** platform-neutral CTest checks; and Ubuntu 22.04, Windows MSVC, and macOS
Clang each built, installed, discovered, compiled, and ran the experimental static
SDK consumer from a clean prefix. The exact tested-environment boundary is recorded in
[`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md).

| Area | Verified status | Evidence and boundary |
| --- | --- | --- |
| Compiler core | **Remotely validated, independently scoped profiles.** | The Ubuntu Linux profile builds `libsynq_lib.a`, `synqc`, test-only `libsynq_ffi.so`, and passes 37 CTests. Separate Windows MSVC and macOS Clang profiles each pass 26 platform-neutral compiler/CLI/C-ABI CTests. Optional historical targets remain outside these claims. |
| `synqc` CLI | **Remotely validated experimental workflow.** | Supports validation, bounded AST/strict-Hybrid OpenQASM source output, bounded constant evaluation, and bounded local probabilities. It is not a general executor, provider client, REPL, package manager, or stable CLI. |
| Typed source model | **Remotely validated bounded subset.** | Supports documented declarations, typed gates, unnamed/named measurement metadata, Alpha expressions, declaration-only functions, and one zero-parameter one-gate kernel with one later strict-Hybrid source-only call expansion. No callable parameters/returns, local scopes, recursion, nested calls, assignments, mutable state, general expressions, or runtime semantics exist. |
| Bounded Boolean declaration evaluation | **Remotely validated Alpha compile-time subset.** | `synqc --eval-constants` deterministically evaluates immutable top-level Boolean literal/reference/`not`/`and`/`or` trees after static resolution, with default expression-depth `16`, request-wide operation budget `128`, and no short-circuit runtime meaning. Depth and operation exhaustion yield structured diagnostics. No assignment, state mutation, loop/branch execution, target-side expression execution, measurement-result value, or simulation execution is claimed. |
| Alpha semantic environment | **Remotely validated read-only inspection subset.** | `synqc --inspect-semantics` renders resolved top-level classical binding metadata: kind, static type, source line, and earlier-binding dependencies. It does not evaluate general source, produce runtime values, sample measurements, or introduce nested scopes. |
| Named registers | **Remotely validated Alpha subset with bounded local simulation.** | Earlier declared `name[index]` operands resolve, strict-Hybrid export preserves declared registers, and the simulator maps explicit register declarations into a declaration-order bounded state vector while retaining source provenance. No lifetime, aliasing, deallocation, dynamic allocation, or hardware mapping semantics exist. |
| Bounded if export | **Remotely validated strict source-lowering subset in [Compiler Core #32188899985](https://github.com/TangoSplicer/SynQ/actions/runs/32188899985).** | One literal `if`, `if not true/false` through compile-time folding, one `if` controlled by an earlier top-level Boolean-literal declaration, or `if not <that declaration>`, with one supported typed gate body lowers to OpenQASM 3. Nested negation, binary expressions, aliases, measurement results, `while`, measurement bodies, and execution remain rejected. |
| Local simulation | **Remotely validated bounded probability model.** | One or more explicit declared registers, limited gates, opt-in combined resource limits, and deterministic probability output with source-register provenance. No samples, collapse, noise, control execution, provider, hardware, lifetime, or dynamic allocation behavior. |
| C ABI and static SDK | **Remotely validated experimental static path on Ubuntu, Windows, and macOS.** | Ubuntu 22.04, Windows MSVC, and macOS Clang clean-install jobs each prove static library/header/CMake package discovery and the external C consumer. No frozen ABI, shared-library delivery, registry, or general cross-platform SDK guarantee is made. |
| Bounded callable-kernel export | **Remotely validated strict source-lowering subset in [Compiler Core #32191872033](https://github.com/TangoSplicer/SynQ/actions/runs/32191872033).** | One earlier zero-parameter `kernel` with exactly one non-parameterized default-register gate body, plus one later `call`, expands into the typed gate during strict Hybrid OpenQASM source generation. No function bodies, arguments, returns, named-register operands, recursion, nested calls, control bodies, simulation, ABI execution, or runtime dispatch are claimed. |
| Interoperability proofs | **Remotely exercised on the Ubuntu full profile.** | C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend fixtures use the opaque C ABI. The Windows profile intentionally excludes those toolchain-specific fixtures. They are not released wrappers or language packages. |
| Alpha Rust wrapper | **Remotely exercised source-only adapter on the Ubuntu full profile.** | The dependency-free `synq-alpha` Cargo package owns the opaque program handle through RAII and exposes parse, OpenQASM 3 export, and ABI-identifier helpers. Three Cargo conformance tests pass through CMake against the test/build C ABI shared library. It is not published to a registry, bundled as a shared-library distribution, or a stable Rust API. |
| Governance and maintenance | **Factual baseline published.** | `CHANGELOG.md`, contribution guidance, a security-reporting route, issue templates, support-environment guide, and ABI/distribution policy exist. They do not promise review times, support SLA, signing, or package availability. |

## Explicit non-claims

SynQ currently makes **no verified claim** of a stable language or ABI release,
general runtime, complete type system, general classical runtime/evaluator, callable
execution, IDE, provider integration, quantum-hardware access, noise model,
measurement sampling, cross-platform distribution, performance target,
enterprise support, commercial service, or complete project-wide test coverage.

The repository frontend/backend directories and historical documents are not
evidence of a deployed product or compiler-service integration unless a document
states a reproducible, current validation path.

## Ordered remaining work

1. Define mutable-cell, assignment, state-lifetime, and deterministic local
   evaluation rules before accepting executable state updates, loops, or branch
   execution.
2. Define practical bounded quantum routines only after explicit qubit/angle
   parameters, resource aliasing, and non-recursive call effects have a written
   contract.
3. Define measurement-result storage and target-side feedback only after local
   and target representations, result lifetime, and rejection paths are
   independently testable.
4. Define a wider callable contract only after parameters, scopes, returns,
   resource effects, recursion rules, and execution boundaries have separate
   written contracts and tests.
5. Expand language-specific wrappers or package delivery only after ownership,
   compatibility, environment, and conformance commitments are explicit.

## Related records

The detailed evidence trail is in
[`REVIEW_EVIDENCE_2026-08-13.md`](./REVIEW_EVIDENCE_2026-08-13.md). Current
operational gates are in [`OPERATIONAL_READINESS.md`](./OPERATIONAL_READINESS.md),
and the dependency-ordered assessment is in
[`OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md`](./OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md).
