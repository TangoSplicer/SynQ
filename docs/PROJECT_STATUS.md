# SynQ Project Status

**Review date:** 17 August 2026
**Status:** Experimental recovery-profile evidence ledger. This document does
not declare SynQ feature-complete, production-ready, or fully operational.

> **Evidence rule:** a capability is described as verified only when the current
> code path, focused tests, and a cited validation result support it. Checked-in
> source, older roadmap text, or an optional target alone is not sufficient.

## Current verified baseline

 [Compiler Core platform-matrix run #32069791226](https://github.com/TangoSplicer/SynQ/actions/runs/32069791226)
passed all five independent jobs for revision `d1288c5`: the Linux recovery
profile passed **31/31** CTest checks; Windows MSVC and macOS Clang each passed
**23/23** platform-neutral CTest checks; and Ubuntu 22.04 plus Windows MSVC each
built, installed, discovered, compiled, and ran the experimental static SDK
consumer from a clean prefix. The exact tested-environment boundary is recorded in
[`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md).

| Area | Verified status | Evidence and boundary |
| --- | --- | --- |
| Compiler core | **Remotely validated, independently scoped profiles.** | The Ubuntu Linux profile builds `libsynq_lib.a`, `synqc`, test-only `libsynq_ffi.so`, and passes 31 CTests. Separate Windows MSVC and macOS Clang profiles each pass 23 platform-neutral compiler/CLI/C-ABI CTests. Optional historical targets remain outside these claims. |
| `synqc` CLI | **Remotely validated experimental workflow.** | Supports validation, bounded AST/strict-Hybrid OpenQASM source output, bounded constant evaluation, and bounded local probabilities. It is not a general executor, provider client, REPL, package manager, or stable CLI. |
| Typed source model | **Remotely validated bounded subset.** | Supports documented declarations, typed gates, unnamed/named measurement metadata, Alpha expressions, and declaration-only callables. No scopes, assignments, general expressions, callable bodies/calls, or runtime semantics exist. |
| Alpha semantic environment | **Remotely validated read-only inspection subset.** | `synqc --inspect-semantics` renders resolved top-level classical binding metadata: kind, static type, source line, and earlier-binding dependencies. It does not evaluate general source, produce runtime values, sample measurements, or introduce nested scopes. |
| Named registers | **Remotely validated Alpha subset with bounded local simulation.** | Earlier declared `name[index]` operands resolve, strict-Hybrid export preserves declared registers, and the simulator maps explicit register declarations into a declaration-order bounded state vector while retaining source provenance. No lifetime, aliasing, deallocation, dynamic allocation, or hardware mapping semantics exist. |
| Bounded if export | **Remotely validated strict source-lowering subset in [Compiler Core #32069791226](https://github.com/TangoSplicer/SynQ/actions/runs/32069791226).** | One literal `if`, or one `if` controlled by an earlier top-level Boolean-literal declaration, with one supported typed gate body lowers to OpenQASM 3. Expressions, aliases, measurement results, `while`, measurement bodies, and execution remain rejected. |
| Local simulation | **Remotely validated bounded probability model.** | One or more explicit declared registers, limited gates, opt-in combined resource limits, and deterministic probability output with source-register provenance. No samples, collapse, noise, control execution, provider, hardware, lifetime, or dynamic allocation behavior. |
| C ABI and static SDK | **Remotely validated experimental static path on Ubuntu and Windows.** | Ubuntu 22.04 and Windows MSVC clean-install jobs each prove static library/header/CMake package discovery and the external C consumer. The macOS job is smoke-only. No frozen ABI, shared-library delivery, registry, macOS SDK, or general cross-platform SDK claim is made. |
| Interoperability proofs | **Remotely exercised on the Ubuntu full profile.** | C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend fixtures use the opaque C ABI. The Windows profile intentionally excludes those toolchain-specific fixtures. They are not released wrappers or language packages. |
| Alpha Rust wrapper | **Remotely exercised source-only adapter on the Ubuntu full profile.** | The dependency-free `synq-alpha` Cargo package owns the opaque program handle through RAII and exposes parse, OpenQASM 3 export, and ABI-identifier helpers. Three Cargo conformance tests pass through CMake against the test/build C ABI shared library. It is not published to a registry, bundled as a shared-library distribution, or a stable Rust API. |
| Governance and maintenance | **Factual baseline published.** | `CHANGELOG.md`, contribution guidance, a security-reporting route, issue templates, support-environment guide, and ABI/distribution policy exist. They do not promise review times, support SLA, signing, or package availability. |

## Explicit non-claims

SynQ currently makes **no verified claim** of a stable language or ABI release,
general runtime, complete type system, general classical evaluator, callable
execution, IDE, provider integration, quantum-hardware access, noise model,
measurement sampling, cross-platform distribution, performance target,
enterprise support, commercial service, or complete project-wide test coverage.

The repository frontend/backend directories and historical documents are not
evidence of a deployed product or compiler-service integration unless a document
states a reproducible, current validation path.

## Ordered remaining work

1. Define target-side Boolean-expression evaluation, writes, lifetime, and
   measurement-result storage before accepting Boolean expressions, aliases,
   loops, branch blocks, or measurement bodies.
2. Design callable bodies/calls, scopes, resource effects, and bounded execution
   separately, after the control-flow contract is testable.
3. Add a macOS clean-install static-SDK conformance job before making any macOS
   SDK or general cross-platform packaging claim.
4. Expand language-specific wrappers or package delivery only after ownership,
   compatibility, environment, and conformance commitments are explicit.

## Related records

The detailed evidence trail is in
[`REVIEW_EVIDENCE_2026-08-13.md`](./REVIEW_EVIDENCE_2026-08-13.md). Current
operational gates are in [`OPERATIONAL_READINESS.md`](./OPERATIONAL_READINESS.md),
and the dependency-ordered assessment is in
[`OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md`](./OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md).
