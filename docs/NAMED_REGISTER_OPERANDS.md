# Named Register Operands

**Status:** Remotely validated Alpha-gated bounded language increment, including
bounded multi-register local simulation, in [Compiler Core platform-matrix
#32065348941](https://github.com/TangoSplicer/SynQ/actions/runs/32065348941).
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
| Local simulation | Bounded simulator | Explicit declared registers receive deterministic declaration-order physical offsets in the bounded state vector; source register names and indices remain visible in results. |

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
callable bodies, arbitrary classical expressions, and general runtime execution
remain separate increments.

No claim is made that this profile defines quantum resource lifetime beyond one
simulation input, execution placement, deallocation, aliasing, dynamic resource
allocation, hardware mapping, a stable language syntax, or a general type system.

## Evidence

Revision `23999f4` passed the five independent jobs in [Compiler Core platform
matrix #32065348941](https://github.com/TangoSplicer/SynQ/actions/runs/32065348941):
**28/28** Linux CTests and **21/21** platform-neutral CTests on Windows and
macOS, plus Ubuntu and Windows static-SDK consumer jobs. Focused coverage includes
Alpha-gate rejection, typed AST/Hybrid provenance, resolver declaration-order and
range failures, cross-register strict Hybrid OpenQASM emission, cross-register
Bell simulation with source-register provenance, combined-limit refusal, and
separate-process `synqc --simulate` output. The evidence does not validate
named-register hardware execution, provider integration, noise, sampling, or
device mapping.
