# Bounded Measurement Feedback

**Feature stage:** Implemented Alpha U4 contract.
**Status:** Remotely verified for revision `62baf18` by the six-job [Compiler
Core platform-matrix run #32250265354](https://github.com/TangoSplicer/SynQ/actions/runs/32250265354):
**44/44** Linux recovery-profile CTests, **30/30** Windows MSVC and macOS Clang
platform-neutral CTests, and clean-install static-SDK consumer checks on Ubuntu,
Windows, and macOS. This document defines a deliberately narrow
measurement-to-correction meaning; it does **not** make general measurement
feedback, target execution, or ABI behavior available.

## Purpose and design position

U1 established bounded immutable Boolean evaluation. U2 established a separate
local-only typed mutable-cell workflow. U3 established source-only expansion of
small parameterized quantum routines. U4 is the first proposed slice that
connects a measurement-result binding to one correction gate. Its purpose is to
make a minimal hybrid feedback workflow inspectable, lowerable, and locally
modelable without importing a general runtime.

OpenQASM distinguishes low-level controller instructions from wider external
classical functions, permits scalar `bit` values in `if` conditions, and
describes measurement-compatible classical storage. [1] [2] SynQ deliberately
adopts only one result, one immediate conditional, and one correction gate. It
does not adopt OpenQASM’s general assignments, comparisons, blocks, `else`, or
loops.

> A **U4 feedback pair** is one top-level named measurement followed immediately
> by one top-level `if <same-result> then quantum x <explicit-qubit>` statement.
> The name denotes a typed single-use measurement-result token, not a general
> Boolean variable or a sampled host-language value.

## Required feature gates and accepted source

A U4 source file must opt into all gates needed by its selected source forms:

```synq
#[experimental(feature = "measurement-feedback")]
#[experimental(feature = "classical-control-flow")]
#[experimental(feature = "qubit-declarations")]

qubit q[2]
quantum h q[0]
measure q[0] as observed
if observed then quantum x q[1]
```

When a non-default register occurs in either operand, the existing
`named-qubit-register-operands` gate is also required. The existing
`measure q[index] as name` parser form remains a static-provenance form outside
U4; this contract changes behavior only when a named result participates in the
exact gated feedback pair.

| Element | U4 acceptance rule | Explicit exclusion |
| --- | --- | --- |
| Measurement | Exactly one `measure register[index] as result` in the feedback path; the register and literal index must resolve to an earlier declared, in-range operand. | Multiple named feedback measurements, result reassignment, bare classical result values, and measurement in a control body. |
| Result lifetime | The result must be used by the immediately following top-level `if` as the un-negated, direct identifier condition, then expires. | Aliases, `not result`, `and`/`or`, literal conditions, later uses, reuse, forward use, `let`/`var`/`set` reads, scopes, and blocks. |
| Correction | The body is exactly one `quantum x register[index]` gate. The correction operand must resolve to an earlier declared, in-range qubit. The source and correction qubits may be the same physical operand. | Any other gate, parameters, two-qubit gates, routine calls, a measurement body, `else`, multiple bodies, loops, or subsequent dynamic operations. |
| Program envelope | Ordinary supported gates may precede the measurement. The feedback pair ends the U4 simulator path. | Gates, declarations, additional measurements, calls, state updates, or controls after the correction when U4 local feedback simulation is requested. |

The one-gate `x` restriction is intentional. It supports both a correction on a
separate target and the bounded same-qubit reset pattern `measure q[0] as r`
then `if r then quantum x q[0]`, without allowing a broad dynamic circuit
language.

## Typed representation and resolver contract

The implementation must retain `MeasurementNode::result_name` and
`HybridMeasurement::result_name`; it must not rewrite a result into an
immutable declaration or a mutable cell. The resolver shall retain
`SemanticBindingKind::MeasurementResult`, static Boolean classification, source
span, source measurement operand, and declaration index.

For an enabled U4 feedback pair, the resolver must additionally produce a
dedicated resolved feedback representation containing the measurement binding
index, source operand, correction gate, and correction operand. It must reject
the input before export or simulation when the result binding is unavailable,
is not directly adjacent, is reused, has an unsupported condition shape, has a
non-`x` body, or has an invalid qubit operand. Existing non-U4 named-result
static provenance remains unchanged.

The correction’s `x` gate is not evaluated by the classical evaluator. It is a
quantum operation ordered after a measurement-result token, so it cannot be
lowered as a U2 state write or an U3 routine call.

## Strict Hybrid OpenQASM source lowering

For the exact accepted U4 pair, strict Hybrid export shall generate one scalar
target bit and preserve source order:

```qasm
OPENQASM 3.0;
include "stdgates.inc";
qubit[2] q;
bit synq_measure_observed;
h q[0];
synq_measure_observed = measure q[0];
if (synq_measure_observed) x q[1];
```

`synq_measure_<result>` is exporter-owned target storage. It is distinct from a
SynQ U2 cell, has no public user spelling, and exists only in emitted source.
OpenQASM documents scalar `bit` as a `0`/`1` type and permits scalar `bit` as
an `if` condition, which makes this emitted form a narrow target-language
representation rather than a claim about any hardware backend. [1] [2]

Strict Hybrid export must reject any named measurement that does not participate
in a resolver-accepted U4 pair. It must also defensively reject malformed,
unresolved, reused, or non-adjacent feedback nodes rather than generating a
partial target program. The target-side result bit must be declared exactly
once, and no generic Boolean storage map may be used for it.

## Local feedback simulator meaning

The U4 simulator is a proposed **exact branch-enumeration model**, not a random
sampler. Before the named measurement it uses the existing bounded pure-state
vector rules. At the measurement, it computes `p(0)` and `p(1)`, normalizes a
collapsed branch for each non-zero probability, applies no gate on the `0`
branch and applies `x` on the `1` branch, then returns the weighted final basis
probability distribution.

This provides deterministic local feedback semantics without inventing an
observed host value. A result record may report the source operand, `p(1)`, and
that the one conditional correction was modeled, but it must not report a
sampled bit. A zero-probability branch is omitted before normalization. Existing
simulator qubit and operation limits continue to apply; U4 adds a hard maximum
of one feedback pair per simulation request.

The simulator must continue to reject named results outside the exact U4 pair,
all general controls, `else`, loops, result reuse, routine nodes, mutable state,
and any operation after the correction. This keeps local behavior deterministic
and ensures no syntactic appearance of feedback is mistaken for a general
hybrid executor.

## ABI and CLI boundary

The experimental C ABI must explicitly reject U4 feedback nodes for both
execution and source export until a separate ABI v2 ownership, result-layout,
and compatibility contract exists. The command-line strict Hybrid export and
the local `--simulate` path are the only proposed U4 surfaces. The CLI must
continue to expose diagnostics rather than silently discarding a result or
correction.

## Reserved diagnostics and limits

| Code | Required U4 failure category | User-directed resolution |
| --- | --- | --- |
| `SYNQ-P019` | A source form asserts the U4 feedback gate but does not use the bounded named-measurement/direct-correction syntax. | Use one top-level `measure register[index] as result` and one immediate `if result then quantum x register[index]`. |
| `SYNQ-R007` | A measurement result is missing, forward, reused, non-adjacent, or has an unsupported condition shape in an enabled U4 pair. | Use the exact direct identifier once, immediately after its measurement. |
| `SYNQ-R008` | The U4 correction body is not one valid, in-range `x` gate. | Use `if result then quantum x register[index]` with an earlier declared operand. |
| `SYNQ-H004` | Strict Hybrid export received an unresolved, malformed, or non-U4 feedback node. | Resolve the program successfully and remain within the exact U4 pair. |
| `SYNQ-SIM006` | The bounded simulator received more than one feedback pair or a node outside the modeled feedback envelope. | Simulate one terminal U4 pair only, or use strict source export for the broader documented subset. |
| Feedback pairs | `1` per `--simulate` request | One branch split bounds the local model to two outcomes. |
| Correction gates | `1` per feedback pair | The direct `x` body prevents nested or uncontrolled dynamic work. |

## Explicit non-goals

U4 does not add general measurement sampling, arbitrary collapse APIs, result
values exposed to a host, general Boolean execution, aliases, comparison,
classical arithmetic, target-side user storage, mutable state lowering,
`else`, loops, blocks, multiple feedback paths, result arrays, routine calls in
the correction, optimization, noise, hardware submission, provider access,
stable ABI behavior, registry packages, or performance guarantees.

## Verification gate before any availability claim

The implementation includes positive and negative parser, AST, Hybrid IR, resolver, strict-exporter,
bounded-simulator, CLI, and C ABI fixtures. Required coverage includes the
default-register and named-register variants; same-qubit and separate-target
correction; gate absence; malformed named result; missing, forward,
non-adjacent, negated, aliased, and reused results; wrong correction gate;
out-of-range source/target; `else`, loop, state, routine, and post-correction
rejection; exact strict-Hybrid output; independent OpenQASM parser validation;
branch-probability correctness; zero-probability branches; legacy unnamed
measurement preservation; and explicit ABI rejection.

The full local **44/44** recovery suite and the six-job Compiler Core matrix
passed on revision `62baf18`. The project status, changelog, tested-environment
record, usable-language profile, readiness assessments, and public Evidence
Ledger may therefore describe this exact U4 slice as remotely verified, subject
to every explicit non-goal above.

## References

[1] [OpenQASM live specification: Classical instructions](https://openqasm.com/language/classical.html)

[2] [OpenQASM live specification: Types and Casting](https://openqasm.com/language/types.html)

[3] [SynQ bounded measurement-result declarations](./MEASUREMENT_RESULTS.md)

[4] [SynQ U4 OpenQASM research notes](./RESEARCH_U4_OPENQASM_MEASUREMENT_FEEDBACK_2026-08-19.md)
