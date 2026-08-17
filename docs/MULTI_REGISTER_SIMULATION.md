# Bounded Multi-Register Simulation Contract

**Status:** Implementation contract for the experimental local probability
simulator.  
**Scope:** Explicit declared registers, supported gates, and trailing unnamed
measurements only. This is not a provider adapter, hardware mapper, noise model,
measurement sampler, or general runtime.

## Purpose

Named register operands already preserve source identity through resolution and
strict Hybrid OpenQASM export. The bounded simulator now supports the same
declared-register form without silently changing the source model. It uses one
deterministic internal pure-state vector whose physical qubit positions are
assigned in source declaration order.

## Allocation rule

Every simulation input must contain one or more explicit positive-size
`qubit name[n]` declarations. Let the declarations in source order be
`r0[n0]`, `r1[n1]`, and so on. The simulator assigns:

| Source register | Physical offset | Valid physical range |
| --- | --- | --- |
| `r0[n0]` | `0` | `0` through `n0 - 1` |
| `r1[n1]` | `n0` | `n0` through `n0 + n1 - 1` |
| `rk[nk]` | `sum(n0 ... n(k-1))` | offset through `offset + nk - 1` |

The total declared size must be from `1` through the configured `max_qubits`
limit. A source operand `name[index]` maps to `physical_offset(name) + index`.
The source name and source index remain attached to each measurement result;
flattening is an internal deterministic state-vector representation only.

## Accepted subset

The simulator accepts declared default or named registers, supported gates
`h`, `x`, `y`, `z`, `cx`, `bell_pair`, `rx`, `ry`, `rz`, and `p`, followed by
unnamed measurements. Cross-register two-qubit gates are allowed when both
operands resolve to distinct physical qubits. Declaration order is resource
allocation order; no implicit allocation, deallocation, aliasing, slicing, or
whole-register operation exists.

## Explicit rejection boundary

Named measurement results, `let` declarations, control flow, callable
declarations, unsupported gates, gates after a measurement, register references
without a declaration, and any total qubit count above `max_qubits` are rejected.
The simulator reports deterministic ideal probabilities only. It does not sample,
collapse, assign a Boolean measurement result, model noise, access hardware, or
resolve a provider layout.

## Result presentation

The `synqc --simulate` output reports the combined qubit count, each source
register’s size and physical offset, nonzero basis probabilities, and each
measurement as `name[index]` with its ideal probability of one. Existing default
register output remains `q[index]`.
