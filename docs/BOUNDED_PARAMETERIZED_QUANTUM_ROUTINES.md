# Bounded Parameterized Quantum Routines

**Feature stage:** Implemented Alpha U3 contract.
**Status:** Remotely verified for revision `be18e0f` by the six-job [Compiler Core
platform-matrix run #32247154982](https://github.com/TangoSplicer/SynQ/actions/runs/32247154982):
41/41 Linux recovery-profile CTests, 28/28 Windows MSVC and macOS Clang
platform-neutral CTests, and clean-install static-SDK consumer checks on Ubuntu,
Windows, and macOS. This document defines a small source-expansion target; it does
**not** make routine execution, target-side
subroutines, simulation execution, ABI execution, or hardware behavior
available.

## Purpose and design position

U1 made immutable Boolean computation inspectable. U2 added a separately
bounded local-only classical cell store. U3 adds the smallest remotely verified reusable quantum
abstraction that can be checked without scopes, general calls, or ownership
inference: one named routine with typed angle and/or qubit formals, one typed
gate body, and one later top-level invocation over caller-owned qubits.

> A **U3 routine** is a compile-time-known, non-recursive one-gate template.
> Its call substitutes one accepted literal angle and explicit declared qubit
> operands into that gate during strict Hybrid OpenQASM source generation.

OpenQASM distinguishes angle parameters from quantum arguments, requires a
given underlying qubit to be passed at most once to a subroutine, requires
definitions before use, and does not allow qubit declarations inside
subroutines.[1] [2] [3] SynQ adopts only the narrow rules it can prove in its
current top-level recovery architecture.

| Design objective | U3 rule | Deliberate exclusion |
| --- | --- | --- |
| Preserve caller ownership | Every actual qubit is a literal earlier-declared `register[index]` operand. | No allocation, capture, alias, register parameter, dynamic indexing, lifetime, or hardware mapping. |
| Preserve typed substitution | A formal is either one `angle` or one `qubit`; a formal qubit appears only as a whole gate operand. | No inferred types, casts, arrays, slices, references, overloads, or generic parameters. |
| Preserve bounded expansion | A routine has one gate body and cannot call a routine. | No nested calls, recursion, mutual recursion, control bodies, return values, stack frames, or runtime dispatch. |
| Preserve resource safety | A two-qubit call must use two distinct physical source operands. | No aliasing through alternate syntax or compiler-selected temporary qubits. |

## Surface syntax

U3 introduces the `parameterized-quantum-routines` Alpha feature. A parameterized
routine requires the existing `callable-declarations` and
`parameterized-quantum-gates` Alpha opt-ins as well.

```synq
#[experimental(feature = "callable-declarations")]
#[experimental(feature = "parameterized-quantum-gates")]
#[experimental(feature = "parameterized-quantum-routines")]
#[experimental(feature = "qubit-declarations")]

qubit q[2]
kernel rotate(angle theta, qubit target) { quantum rz(theta) target }
kernel entangle(qubit control, qubit target) { quantum cx control, target }
call rotate(pi/2, q[0])
call entangle(q[0], q[1])
```

The declaration and invocation grammar is intentionally one-line and has no
semicolon requirement beyond the recovery parser's existing optional trailing
semicolon handling.

```text
routine-declaration := kernel identifier "(" routine-formal-list ")" "{" one-gate-body "}"
routine-formal-list := angle-formal [ "," qubit-formal ] | qubit-formal [ "," qubit-formal ]
angle-formal       := "angle" identifier
qubit-formal       := "qubit" identifier
routine-call       := call identifier "(" routine-actual-list ")"
```

The accepted forms are exactly those in the following table.

| Signature | Sole accepted body | Call actuals | Static effect |
| --- | --- | --- | --- |
| `kernel name(angle theta, qubit target)` | `quantum rx(theta) target`, `ry(theta) target`, `rz(theta) target`, or `p(theta) target` | One accepted existing literal-angle spelling, then one earlier `register[index]`. | One one-qubit rotation/phase gate. |
| `kernel name(qubit target)` | `quantum h target`, `x target`, `y target`, or `z target` | One earlier `register[index]`. | One one-qubit fixed gate. |
| `kernel name(qubit control, qubit target)` | `quantum cx control, target` | Two earlier `register[index]` operands that resolve to distinct source qubits. | One two-qubit controlled-X gate. |

The existing zero-parameter bounded-kernel form remains unchanged. U3 does not
reinterpret an existing `kernel name()` or `call name()` program. A call's
actual list must match its earlier routine's ordered signature exactly. The
accepted angle spellings are the existing literal-angle subset used by bounded
parameterized quantum gates; identifier, mutable-cell, measurement, arithmetic,
decimal, and runtime angle sources are invalid.

## Definition, scope, and call graph

Routine names participate in the existing global top-level uniqueness rule.
All formals are local only to the one body gate. The body may refer to no global
classical binding, mutable cell, measurement result, callable, or qubit name;
its complete quantum vocabulary is its own formal list. A qubit declaration may
not appear in a routine body.

Definitions must precede calls. The U3 body grammar contains only one gate and
no `call` form, so its call graph has no routine-to-routine edges and is
non-recursive by construction. This is a stronger first bound than attempting
to infer a general call graph or permit a recursive runtime model.

For an invocation, the resolver records the referenced earlier routine, checks
the actual count and kind, resolves each source operand against prior qubit
declarations, and rejects a repeated `(register-name, index)` pair in a
two-qubit call. Distinct declared registers denote distinct source resources in
the current declaration model; aliases and dynamic allocation are not part of
U3.

## Lowering and resource bounds

The parser must preserve routine formal order, formal kind, body gate kind,
formal angle reference where applicable, formal qubit operands, and source
spans. Hybrid IR must preserve this information rather than inserting a fake
top-level gate at declaration time.

The strict Hybrid OpenQASM exporter may lower only a resolved U3 routine call.
It substitutes the already accepted literal angle and actual source operands
into the one typed body gate in source order. For example, the `rotate` call
above emits `rz(pi/2) q[0];`; it does not emit an OpenQASM `def`, invoke a
runtime routine, allocate storage, or execute a SynQ call.

To keep output expansion bounded, one strict-Hybrid export accepts at most **32
U3 routine declarations** and **128 U3 routine calls**. The declaration count
and call count are separate from U2 local-state limits. Exceeding either limit
is an export error and produces no partial output. U3 does not add an evaluator
mode, and the bounded quantum simulator must reject U3 declarations and calls
until a separate routine-simulation contract exists.

## Required representation and rejection behavior

| Layer | Required U3 representation | Required rejection behavior |
| --- | --- | --- |
| Parser AST | Typed ordered `angle`/`qubit` formal list, one typed formal-based gate body, and an ordered call actual list. | Reject missing type/name, duplicate formals, malformed list, unsupported body, missing gate formal use, or a call without all required gates. |
| Hybrid IR | Parameterized-routine declaration and call nodes that retain formals, symbolic body operands, actual literal angle/operands, and spans. | Do not coerce formals into top-level operands or silently lower malformed calls. |
| Resolver | Earlier-definition lookup; exact formal/actual arity and kind checks; formal-use validation; distinct actual-qubit validation. | Reject later/unknown target, wrong count/kind, scalar nonliteral, unknown/out-of-range actual, duplicate actual qubit, capture, or non-U3 callable target. |
| Strict Hybrid exporter | One static, source-ordered gate expansion per resolved U3 call. | Reject declaration/call count overflow and every unexpanded routine node; emit no target-side `def` or runtime construct. |
| Simulator and ABI | Explicit U3-node rejection. | Do not imply routine simulation, call execution, FFI execution, hardware submission, or provider behavior. |

## Reserved diagnostics

The following codes are reserved for U3. Implementation may improve text but
must retain the category and code throughout this Alpha slice.

| Code | Required failure category | User-directed resolution |
| --- | --- | --- |
| `SYNQ-P017` | Malformed parameterized routine declaration or signature. | Use one documented typed signature and one supported formal-only gate body. |
| `SYNQ-P018` | Malformed parameterized routine invocation. | Use `call <earlier-routine>(<matching-actuals>)`. |
| `SYNQ-R004` | Routine is missing, later, unsupported, or has an invalid formal-body relation. | Define a matching U3 routine earlier and use every formal exactly as documented. |
| `SYNQ-R005` | Call actual arity, order, or static kind does not match the routine signature. | Supply the exact literal-angle and/or qubit actual sequence required by the declaration. |
| `SYNQ-R006` | A U3 two-qubit call aliases one underlying source qubit. | Use two distinct declared `register[index]` operands. |
| `SYNQ-H003` | Strict Hybrid routine declaration or call expansion limit is exceeded. | Reduce the U3 routine/call count; do not rely on implicit expansion. |

## Explicit non-goals

U3 does not add general scalar types, integer/float angle values, mutable angle
state, parameter expressions, multiple angle formals, registers/arrays as
parameters, aliases, implicit capture, local declarations, routine-local
qubits, nested calls, recursion, returns, function bodies, control flow,
measurement, feedback, target-side `def` emission, target-side assignment,
routine simulation, ABI execution, dynamic allocation, optimization, hardware
mapping, provider integration, package delivery, or stability promises.

## Verification gate before any availability claim

The implementation includes positive and
negative parser, typed-AST, Hybrid-IR, resolver, strict-Hybrid exact-output,
CLI, OpenQASM reference-parser, simulator-rejection, and ABI-rejection fixtures.
The matrix must cover all three signatures, earlier-only visibility, every
feature-gate absence, malformed and duplicate formals, unsupported and captured
body operands, wrong call arity/kind/order, named-register arguments, duplicate
two-qubit actuals, out-of-range operands, declaration/call limits, source-order
expansion, and unchanged zero-parameter kernel behavior.

The implementation revision passed the full local **41/41** recovery suite and
the six-job remote Compiler Core matrix. The repository status, changelog,
tested-environment record, usable-language profile, operational readiness
assessment, and public Evidence Ledger may therefore describe this bounded U3
slice as remotely verified, subject to all explicit non-goals above.

## References

[1] [OpenQASM Live Specification — Subroutines](https://openqasm.com/language/subroutines.html)

[2] [OpenQASM Live Specification — Scoping of Variables](https://openqasm.com/language/scope.html)

[3] [OpenQASM 3.0 Specification — Gates](https://openqasm.com/versions/3.0/language/gates.html)

[4] [SynQ U3 routine-design research notes](./RESEARCH_U3_OPENQASM_ROUTINES_2026-08-19.md)
