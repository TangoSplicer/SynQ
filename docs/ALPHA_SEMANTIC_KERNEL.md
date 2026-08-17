# Alpha Semantic-Kernel Contract

**Status:** Bounded implementation contract for the experimental recovery profile.  
**Scope:** Top-level static binding identity and inspection only. This document
does not define a general SynQ runtime, assignment, nested scope, closure,
function body, or execution model.

## Purpose

SynQ already parses typed classical declarations, typed measurement-result names,
typed quantum operations, and constrained control conditions. The recovery
resolver validates earlier references and selected static types, but users cannot
yet inspect a single explicit semantic environment. This contract adds a small,
read-only semantic kernel so every accepted top-level classical binding has a
stable identity, kind, static type, source span, and declared dependency list.

> The semantic environment is descriptive. It never evaluates arbitrary source,
> allocates qubits, runs a control branch, samples a measurement, or changes a
> program’s execution behavior.

## Binding model

The bounded recovery profile has exactly one top-level binding environment. A
binding enters it in source order and is immutable after creation. Existing
parser-time uniqueness checks remain the front-door rule: a top-level name cannot
be reused by a `let`, a qubit declaration, a callable declaration, or a named
measurement result. The semantic environment records only classical `let` and
named-measurement-result bindings; quantum registers and callable declarations
remain separate resource and callable namespaces pending their own contracts.

| Binding source | Semantic binding kind | Static type | Effect | Included in this increment |
| --- | --- | --- | --- | --- |
| `let name = <literal>` | `Value` | Integer, Decimal, Boolean, or String | Immutable value declaration | Yes |
| `let name = earlier_name` | `Value` | Inherited from earlier binding | Immutable alias; no general evaluation | Yes |
| `let name = <bounded Integer arithmetic>` | `Value` | Integer | Statically constrained expression; evaluation remains opt-in | Yes |
| `let name = <opaque text>` | `Value` | Unknown | No inferred or invented semantics | Yes |
| `measure q[i] as name` | `MeasurementResult` | Boolean | Declares measurement-result metadata; no sampled value | Yes |
| `qubit name[n]` | — | — | Resource semantics deferred | No |
| `fn name()` / `kernel name()` | — | — | Callable-body and call semantics deferred | No |

## Public inspection boundary

The supported `synqc` command gains an opt-in `--inspect-semantics` mode. It
parses, lowers, and resolves the bounded profile, then renders the top-level
semantic environment in source order. Each row contains the name, semantic kind,
static type, declaring source line, and prior binding dependencies.

The output is an inspection report, not an evaluation result. `Unknown` means
SynQ deliberately has no static meaning for that source text. A measurement
result shown as Boolean means it may be referenced by the bounded resolver; it
does not mean that local simulation samples or assigns a measurement value.

## Invariants and diagnostics

1. Every environment binding refers to an existing resolved top-level node.
2. Binding order equals source order.
3. Dependencies point only to earlier classical bindings.
4. Static types are the parser/lowering type or an earlier binding’s type; no
   runtime type inference is performed.
5. Existing duplicate-name, forward-reference, and static-type diagnostics retain
   their codes and source provenance.
6. The inspection mode must fail on the same parse/lowering/resolution errors as
   `--validate`; it must not manufacture a partial environment.

## Explicit non-goals

This increment does not add assignment, mutability, shadowing, nested/block
scope, value storage, dynamic dispatch, callable bodies/calls, general
expressions, local execution, resource lifetime, hardware execution, or stable
ABI surface. Named-register resource semantics and multi-register simulation are
the next separate language-core contract.
