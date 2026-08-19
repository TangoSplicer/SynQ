# SynQ Operational Readiness Definition

**Status:** Planning and acceptance record. This document does not declare SynQ
operationally ready.
**Last reviewed:** 19 August 2026

## Purpose

SynQ’s completed typed-foundation sequence proves that the recovery compiler can
parse, preserve, validate, and export a deliberately bounded subset. A usable
programming language requires more: a reliable developer workflow, precisely
defined semantics, repeatable execution or simulation, distribution, performance
evidence, and a maintenance model.

> **Operationally ready** means that a documented supported workflow can be
> installed, run, tested, diagnosed, and maintained with measured boundaries. It
> does not mean feature-complete, universally interoperable, or suitable for all
> quantum workloads.

## Current baseline

| Area | Current evidence | Operational gap |
| --- | --- | --- |
| Compiler foundation | [Compiler Core #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516) passed 45/45 Linux recovery-profile checks and 31/31 Windows/macOS platform-neutral checks for revision `7307c00`, plus Ubuntu 22.04, Windows MSVC, and macOS Clang static-SDK consumer jobs. | The grammar and semantics remain intentionally narrow. |
| Source model | Typed gates, bounded classical metadata/evaluation, qubit declarations with Alpha named operands, documented `if` source-lowering forms, bounded simulation, U3 routines, U4 feedback, and U5 one-formal local callable evaluation exist behind exact gates/opt-ins. | No nested negation over results, binary-expression/alias/`else`/loop result control, general scopes/returns/recursion/nested calls, routine execution, or general executable classical semantics exists. |
| Quantum path | AST/strict Hybrid OpenQASM export and a bounded local probability simulator are tested for exact supported subsets, including U4 source feedback and two-branch local feedback enumeration. | Source lowering and local simulation are not provider integration or hardware execution. |
| Native interoperability | C, direct Rust, source-only Alpha Rust wrapper, test-only Common Lisp, test-only Clojure/JNA, and test-only Mercury consumers exercise an opaque C ABI. | There are no registry packages, stable safe wrappers, or a frozen ABI policy. |
| Command-line workflow | `synqc` validates, exports supported OpenQASM subsets including static U3 source expansion, evaluates constants/state/U5 local callables, inspects static bindings, and simulates bounded probabilities. | It is an experimental recovery-profile command, not a general executor, provider client, or stable CLI contract. |
| Frontend | The frontend has 33 local unit tests and a successful production build. | It is not evidence of a compiled-language runtime or deployed backend integration. |
| Performance | A local opt-in deterministic bounded-simulator benchmark and experimental install/archive path exist. | No claim can be made that SynQ matches Python’s execution performance or Java’s ecosystem/runtime benefits. |

## Acceptance gates

| Gate | Required evidence before the gate can be marked complete |
| --- | --- |
| G1 — Supported CLI | **Remotely validated.** `synqc` parses a documented file, reports structured failures, emits one supported output, returns documented exit codes, and has end-to-end tests in [Compiler Core #42](https://github.com/TangoSplicer/SynQ/actions/runs/31886580043). |
| G2 — Executable semantic slice | **Remotely validated.** The bounded evaluators and local probability simulator have explicit opt-ins, resource limits, negative tests, one U4 terminal two-branch feedback model, and U5 one-formal local callable evaluation; the current 45-check Ubuntu recovery profile passed in [Compiler Core #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516). |
| G3 — Quantum developer loop | **Remotely validated.** A developer can create one verified example, validate it, inspect/export it, and calculate bounded local probabilities without hardware claims in [Compiler Core #43](https://github.com/TangoSplicer/SynQ/actions/runs/31886881473). |
| G4 — Tooling and diagnostics | **Remotely validated compiler baseline.** Versioned CLI smoke coverage, structured failures, source-aware diagnostics, reference documentation, and the documented CMake/CTest build path were compiled and tested in [Compiler Core #45](https://github.com/TangoSplicer/SynQ/actions/runs/31887461976). Stable output compatibility and wider examples remain gaps. |
| G5 — Interoperability contract | **Remotely validated experimental foundation with published policy and SDK conformance.** The versioned opaque C ABI and each advertised test consumer are independently exercised by the recovery profile, including [Compiler Core #46](https://github.com/TangoSplicer/SynQ/actions/runs/31898767207). [`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md) defines the v1 change, ownership, and delivery boundary; [`EXPERIMENTAL_NATIVE_SDK.md`](./EXPERIMENTAL_NATIVE_SDK.md) records remote clean-prefix static-SDK conformance. Supported-platform evidence, packages, and safe wrappers remain gaps. |
| G6 — Performance and reliability | **Remotely validated build/test configuration; local-only performance observation.** The warnings/hardening-enabled recovery compiler profile and its 45 tests passed in [Compiler Core #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516); the fixed benchmark timing is not a CI gate. Regression thresholds, sanitizers, and broader security review remain gaps. |
| G7 — Distribution and maintenance | **Remotely validated compiler baseline; locally validated delivery operations.** The versioned CLI and package configuration compile with the 45-check recovery profile in [Compiler Core #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516), while installation and CPack archive generation were run locally. Tags, signing, support policy, security disclosure route, and a maintained known-issues ledger remain gaps. |

## Performance and ecosystem principles

SynQ will not make a blanket promise to be “as fast as Python” because Python’s
performance depends on workload, interpreter, native extensions, and deployment
model. Instead, SynQ should establish representative benchmarks for parse time,
typed lowering, supported export, and any future local simulator; each claim must
name its hardware, build flags, input size, baseline, and measurement method.

The project can pursue Java-like benefits—clear APIs, portability, tooling,
diagnostics, compatibility policy, and long-lived documentation—without claiming
Java compatibility or reproducing the Java ecosystem. Those benefits are product
qualities, not a shortcut around independently measured implementation work.

## Ordered implementation strategy

The next buildable path must remain intentionally small: U5 verifies one local
callable frame and U4 verifies one feedback pair, but any wider callable or
feedback model must first specify result multiplicity/lifetime, `else`, scope,
returns, target representation, simulator semantics, error behavior, and tests
together. Neither U5 local evaluation nor U4 source lowering is evidence of a
general runtime.

## Non-claims

At this stage SynQ is not a fully operational language project. It does not have
a supported runtime, simulator, hardware connector, general package manager,
stable distributed ABI, complete type system, general classical evaluator,
benchmark-backed performance target, or release distribution. This record makes
the remaining work measurable rather than obscuring it.
