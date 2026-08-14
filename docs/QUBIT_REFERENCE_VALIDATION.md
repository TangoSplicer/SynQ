# Bounded Default-Register Qubit Reference Validation

**Status:** Locally validated extension; no remote validation claim is made in
this record.
**Last reviewed:** 14 August 2026

## Purpose

This extension adds static source validation for the existing `q[index]` operand
surface only when a program explicitly declares the default register as
`qubit q[n]`. It is a resolver check over typed Hybrid IR; it does not allocate,
simulate, or execute qubits.

> **Design rule:** an explicit `qubit q[n]` declaration makes later `q[index]`
> uses checkable. It does not change the legacy behavior of a program that has
> no explicit default-register declaration.

## Bounded behavior

| Program shape | Resolver result |
| --- | --- |
| No `qubit q[n]` declaration | Existing indexed gate and measurement operands retain their prior behavior. |
| `qubit q[2]` before `quantum h q[1]` or `measure q[1]` | The operand is accepted as within the inclusive range `q[0]` through `q[1]`. |
| `q[index]` before a later `qubit q[n]` declaration | `SYNQ-Q001` rejects the forward default-register use. |
| `qubit q[1]` followed by `quantum h q[1]` or `measure q[1]` | `SYNQ-Q002` rejects the out-of-range operand. |
| A bounded Alpha control-flow body using `q[index]` | The typed gate or measurement body receives the same order/range check. |

Only the literal default name `q` participates because the existing operand
grammar contains only `q[index]`, not named-register operands. A declaration
such as `qubit register[2]` remains typed metadata but cannot yet be referenced
by a quantum operation.

## Explicit non-goals

This extension does not implement named-register operands, runtime allocation,
qubit ownership, aliasing, liveness, deallocation, borrow checking, qubit
mapping, multi-register range checks, execution, simulation, provider
integration, hardware submission, or OpenQASM declaration lowering.
