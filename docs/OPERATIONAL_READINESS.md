# SynQ Operational Readiness Definition

**Status:** Planning and acceptance record. This document does not declare SynQ
operationally ready.
**Last reviewed:** 15 August 2026

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
| Compiler foundation | [Compiler Core #40](https://github.com/TangoSplicer/SynQ/actions/runs/31849787206) passed 23/23 recovery-profile checks. | The grammar and semantics remain intentionally narrow. |
| Source model | Typed gates, bounded classical metadata, qubit declarations, named measurement metadata, and declaration-only callables exist behind feature gates. | No general expression evaluator, scopes, callable bodies/calls, or executable classical semantics exists. |
| Quantum path | AST OpenQASM export and a stricter internal Hybrid IR export are tested for exact supported subsets. | Export is source generation only; it is not simulation, provider integration, or hardware execution. |
| Native interoperability | C, Rust, test-only Common Lisp, test-only Clojure/JNA, and test-only Mercury consumers exercise an opaque C ABI. | There are no distributed language packages, safe wrappers, or a frozen ABI policy. |
| Command-line workflow | The historical compiler CLI remains disabled because it depends on incomplete runtime/exporter subsystems. | No supported end-to-end `synqc` developer command exists yet. |
| Frontend | The frontend has 33 local unit tests and a successful production build. | It is not evidence of a compiled-language runtime or deployed backend integration. |
| Performance | No benchmark suite or performance target exists. | No claim can be made that SynQ matches Python’s execution performance or Java’s ecosystem/runtime benefits. |

## Acceptance gates

| Gate | Required evidence before the gate can be marked complete |
| --- | --- |
| G1 — Supported CLI | A versioned `synqc` command parses a documented file, reports structured failures, emits one supported output, returns stable exit codes, and has end-to-end tests. |
| G2 — Executable semantic slice | A small, documented subset has deterministic local evaluation or simulation semantics, explicit resource limits, and negative/error tests. |
| G3 — Quantum developer loop | A developer can create one verified example, validate it, inspect/export it, and run it against a local supported engine without hardware claims. |
| G4 — Tooling and diagnostics | Stable CLI output, source locations, reference documentation, examples, and a reproducible build/test command are maintained together. |
| G5 — Interoperability contract | Versioning, ownership, compatibility, supported constructs, and failure behavior are documented and separately tested for each advertised consumer. |
| G6 — Performance and reliability | Repeatable benchmark fixtures, regression thresholds, sanitizers or equivalent checks, dependency policy, and release checks produce recorded evidence. |
| G7 — Distribution and maintenance | Tagged release artifacts or reproducible install instructions, a support/contribution policy, security disclosure route, and a known-issues ledger exist. |

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

The next buildable path is intentionally small: restore a new recovery-profile
CLI around the validated parser and bounded OpenQASM exporter, rather than
repairing the historical runtime-dependent entry point. A subsequent semantic
slice can add deterministic local behavior only after its source language,
resource model, error behavior, and tests are designed together.

## Non-claims

At this stage SynQ is not a fully operational language project. It does not have
a supported runtime, simulator, hardware connector, general package manager,
stable distributed ABI, complete type system, general classical evaluator,
benchmark-backed performance target, or release distribution. This record makes
the remaining work measurable rather than obscuring it.
