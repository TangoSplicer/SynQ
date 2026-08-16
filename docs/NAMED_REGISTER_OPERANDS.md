# Named Register Operands

**Status:** Remotely validated Alpha-gated bounded language increment in
[Compiler Core #47](https://github.com/TangoSplicer/SynQ/actions/runs/31951911553).
This document describes the implemented recovery-profile contract and its
remaining boundaries.

## Purpose and syntax

The recovery profile already parses `qubit <identifier>[positive-size]`
declarations but historically accepts only `q[index]` as a gate or measurement
operand. The named-register increment adds the source form:

```synq
#[experimental(feature = "qubit-declarations")]
#[experimental(feature = "named-qubit-register-operands")]
qubit data[2]
qubit ancilla[1]
quantum h data[0]
quantum cx data[0], ancilla[0]
measure data[1]
```

The new operand form is intentionally limited to one declared register name and
one literal non-negative integer index. It does not add slices, whole-register
operations, aliases, indirect indexes, arrays of registers, allocation at
runtime, scopes, deallocation, or provider semantics.

## Safety contract

| Rule | Enforcement point | Expected outcome |
| --- | --- | --- |
| Opt-in | Parser | Any non-default register operand requires `named-qubit-register-operands` Alpha annotation. |
| Declaration order | Resolver | A register must be declared before it is referenced. |
| Name validity | Resolver | A referenced register must exactly match an earlier `qubit` declaration. |
| Literal bounds | Resolver | Each `name[index]` must satisfy `index < declared size`. |
| Duplicate declarations | Existing parser rule | A qubit register shares the existing top-level namespace and cannot be redeclared. |
| Strict export | Hybrid OpenQASM 3 exporter | Each declared register is emitted as `qubit[n] name;`; only supported gates and unnamed measurements are lowered. |
| Local simulation | Bounded simulator | The current simulator remains default-register-only and rejects named-register programs rather than inventing a flattening model. |

The default operand form `q[index]` preserves its existing behavior. If an
explicit `qubit q[n]` declaration appears, it remains subject to declaration
order and range validation. Existing legacy source without any explicit default
declaration also retains its recovery-profile parser/resolver behavior.

## Diagnostics

The implementation extends the existing `SYNQ-Q001` and `SYNQ-Q002` family
without treating a register reference as a general variable. The diagnostic must
name the actual register and index, identify the declaration expectation or
declared range, and point users toward the required Alpha annotation where the
syntax is gated.

## OpenQASM and non-goals

The strict Hybrid exporter may lower multiple declared registers directly because
OpenQASM 3 accepts named qubit declarations. Unnamed measurements are emitted
into deterministic per-register classical declarations, while named SynQ
measurement results remain outside the exporter boundary. Conditional nodes,
callable bodies, arbitrary classical expressions, and simulator support remain
separate increments.

No claim is made that this profile defines quantum resource lifetime, execution
placement, multi-register simulation, hardware mapping, a stable language
syntax, or a general type system.

## Evidence

Revision `3a87e63` passed **27/27** recovery-profile CTest checks in [Compiler
Core #47](https://github.com/TangoSplicer/SynQ/actions/runs/31951911553). The
focused coverage includes Alpha-gate rejection, typed AST/Hybrid provenance,
resolver declaration-order and range failures, cross-register strict Hybrid
OpenQASM emission, a separate-process `synqc --emit-openqasm-hybrid` path, and
an explicit bounded-simulator rejection fixture. The evidence does not validate
named-register hardware execution, provider integration, or multi-register
simulation.
