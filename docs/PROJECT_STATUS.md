# SynQ Project Status

**Review date:** 16 August 2026
**Status:** Experimental recovery-profile evidence ledger. This document does
not declare SynQ feature-complete, production-ready, or fully operational.

> **Evidence rule:** a capability is described as verified only when the current
> code path, focused tests, and a cited validation result support it. Checked-in
> source, older roadmap text, or an optional target alone is not sufficient.

## Current verified baseline

 [Compiler Core platform-matrix run #32018610062](https://github.com/TangoSplicer/SynQ/actions/runs/32018610062)
passed all five independent jobs for revision `fb1d2b4`: the Linux recovery
profile passed **27/27** CTest checks; Windows MSVC and macOS Clang each passed
**20/20** platform-neutral CTest checks; and Ubuntu 22.04 plus Windows MSVC each
built, installed, discovered, compiled, and ran the experimental static SDK
consumer from a clean prefix. The exact tested-environment boundary is recorded in
[`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md).

| Area | Verified status | Evidence and boundary |
| --- | --- | --- |
| Compiler core | **Remotely validated, independently scoped profiles.** | The Ubuntu Linux profile builds `libsynq_lib.a`, `synqc`, test-only `libsynq_ffi.so`, and passes 27 CTests. Separate Windows MSVC and macOS Clang profiles each pass 20 platform-neutral compiler/CLI/C-ABI CTests. Optional historical targets remain outside these claims. |
| `synqc` CLI | **Remotely validated experimental workflow.** | Supports validation, bounded AST/strict-Hybrid OpenQASM source output, bounded constant evaluation, and bounded local probabilities. It is not a general executor, provider client, REPL, package manager, or stable CLI. |
| Typed source model | **Remotely validated bounded subset.** | Supports documented declarations, typed gates, unnamed/named measurement metadata, Alpha expressions, and declaration-only callables. No scopes, assignments, general expressions, callable bodies/calls, or runtime semantics exist. |
| Named registers | **Remotely validated Alpha subset in [Compiler Core #47](https://github.com/TangoSplicer/SynQ/actions/runs/31951911553).** | Earlier declared `name[index]` operands resolve and strict-Hybrid export supports declared registers. The bounded simulator rejects the named-register form. |
| Literal-if export | **Remotely validated strict source-lowering subset in [Compiler Core #48](https://github.com/TangoSplicer/SynQ/actions/runs/31952214849).** | One literal `if` with one supported typed gate body lowers to OpenQASM 3. Identifier/expression conditions, `while`, measurement bodies, and execution remain rejected. |
| Local simulation | **Remotely validated bounded probability model.** | One default register, limited gates, opt-in limits, and deterministic probability output only. No samples, collapse, noise, control execution, provider, or hardware behavior. |
| C ABI and static SDK | **Remotely validated experimental static path on Ubuntu and Windows.** | Ubuntu 22.04 and Windows MSVC clean-install jobs each prove static library/header/CMake package discovery and the external C consumer. The macOS job is smoke-only. No frozen ABI, shared-library delivery, registry, macOS SDK, or general cross-platform SDK claim is made. |
| Interoperability proofs | **Remotely exercised on the Ubuntu full profile.** | C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend fixtures use the opaque C ABI. The Windows profile intentionally excludes those toolchain-specific fixtures. They are not released wrappers or language packages. |
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

1. Define a coherent typed semantic kernel—values, scopes, binding rules,
   effects, and IR invariants—before broadening syntax or backend claims.
2. Decide a separate resource and simulation contract before attempting
   multi-register simulation or allocation semantics.
3. Design non-literal conditions, loops, branch blocks, and measurement bodies
   only with target-side classical storage and execution semantics.
4. Design callable bodies/calls, scopes, and bounded execution separately.
5. Add any language-specific wrapper only after package, ownership, compatibility,
   environment, and conformance commitments are explicit.

## Related records

The detailed evidence trail is in
[`REVIEW_EVIDENCE_2026-08-13.md`](./REVIEW_EVIDENCE_2026-08-13.md). Current
operational gates are in [`OPERATIONAL_READINESS.md`](./OPERATIONAL_READINESS.md),
and the dependency-ordered assessment is in
[`OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md`](./OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md).
