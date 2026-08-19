# SynQ Usable Experimental-Language Profile

**Status:** Design and prioritization record for the next language-core
increments. It does not claim a stable release, a general runtime, or feature
availability beyond the evidence stated below.

## Purpose

SynQ’s near-term objective is an **usable experimental hybrid programming
language**, not a broad syntax catalogue. A usable program should be able to
declare and evaluate bounded classical values, describe and compose small
quantum routines over explicit qubit resources, and connect the two through
documented measurement and control semantics. Each surface must have a
deterministic local meaning, a strict source-lowering meaning where supported,
and a structured rejection where it does not.

> **Usable** means that a developer can write, validate, inspect, evaluate or
> simulate the documented bounded workflow locally and obtain reproducible
> diagnostics. It does **not** mean hardware execution, arbitrary programs,
> a stable ABI, or production support.

## Current verified starting point

| Layer | Current verified behavior | Current boundary |
| --- | --- | --- |
| Classical | Immutable top-level Boolean, Integer, and String declarations; bounded Boolean and one-operation Integer expression trees; recursively evaluated Boolean declaration expressions with explicit depth/operation budgets; limited strict-Hybrid conditional source lowering. | No assignment, scopes, precedence grammar, loop execution, mutable runtime state, or general runtime semantics. |
| Quantum | Typed gates, explicit declared-register operands, named-register allocation for bounded ideal-probability simulation, measurements, and strict OpenQASM 3 source export. | No noise, sampling/collapse, reset lifecycle, hardware/provider execution, optimization, dynamic allocation, or general resource ownership. |
| Callable composition | One zero-parameter `kernel` with one non-parameterized default-register gate body may be followed by one `call`; strict Hybrid export expands that gate as source. | No parameters, returns, function bodies, named-register callable operands, recursion, nested calls, runtime dispatch, simulation execution, or ABI execution. |
| Hybrid control | Literal and bounded immutable-Boolean `if` source-lowering forms with one gate body. | No measurement feedback, state writes, `else`, loops, branch execution, or general target-control contract. |

The verified test and platform details remain in
[`PROJECT_STATUS.md`](./PROJECT_STATUS.md) and
[`TESTED_ENVIRONMENTS.md`](./TESTED_ENVIRONMENTS.md).

## Staged usable-language target

The work proceeds from values to state, then resources and composition, then
hybrid feedback. This order avoids making a parser form appear executable
before its values, effects, and target representation have a contract.

| Stage | Classical usability | Quantum usability | Hybrid integration | Completion evidence |
| --- | --- | --- | --- | --- |
| U1 — Bounded constant programs | **Completed Alpha Boolean slice:** recursively typed Boolean constant evaluation over immutable top-level bindings with fixed depth and operation limits; existing checked Integer boundary remains separately constrained. | Existing typed gates and declared-register operands remain unchanged. | Compile-time conditions may use only expressions whose bounded evaluator proves a Boolean result; this is not target-side expression execution. | Positive/negative evaluator, resolver, CLI, local full-suite, and six-job remote CI fixtures passed in [Compiler Core #32239066421](https://github.com/TangoSplicer/SynQ/actions/runs/32239066421). |
| U2 — Explicit classical state | Typed mutable cells, assignment, state lifetime, and deterministic local evaluation rules, as proposed in [`BOUNDED_MUTABLE_CLASSICAL_STATE.md`](./BOUNDED_MUTABLE_CLASSICAL_STATE.md). | Quantum operations retain explicit resource validation. | No measurement feedback or loop execution until target storage and branch effects are separately defined. | State-transition fixtures, invalid-write diagnostics, bounded resource tests, and remote CI evidence. |
| U3 — Practical quantum routines | Typed scalar angle and explicit qubit parameters, non-recursive call graph, resource alias rejection, and static source expansion. | Reusable routines over caller-supplied qubits; no allocation inside a routine. | Classical parameters remain compile-time constants initially. | Parser/IR/resolver/exporter conformance, call-graph and alias-negative tests, reference-parser checks, and CI. |
| U4 — Measurement feedback | Typed measurement-result storage with explicit local and target-side representation. | Measurements can feed one bounded conditional correction path. | Defined result lifetime, target Boolean storage, and source-only/local-simulation behavior. | Deterministic simulator fixtures, exact lowered source, negative lifetime/unsupported-target tests, and CI. |

## Design rules

### Classical layer

The first usable classical increment must evaluate the existing typed expression
trees rather than introduce a second evaluator. It will use explicit maximum
expression depth, maximum operation count, fixed-width checked Integer behavior,
and topologically earlier immutable dependencies. Failed evaluation must produce
a diagnostic, never an implicit fallback value. Assignment is a later distinct
increment because it introduces a store, write ordering, lifetime, and mutation
effects.

OpenQASM distinguishes low-level controller operations from external classical
functions, requires assignment operands to have matching types, and documents
operator precedence and associativity. SynQ will take only the small,
deterministic subset that it can validate and simulate locally; it will not claim
the full OpenQASM classical model.[1]

### Quantum layer

Routines must pass quantum resources explicitly. The first parameterized routine
subset will prohibit implicit capture, local qubit allocation, recursion, and
overlapping qubit arguments. These restrictions align with the target
interoperability principle that subroutine qubit arguments are explicit and a
given qubit must not be passed more than once in one call.[2] SynQ will enforce
its smaller static subset before export or simulation, not defer resource errors
to a backend.

### Hybrid layer

Measurement feedback is not just a syntax feature. It needs an explicit bit
storage representation, write point, lifetime, target-lowering representation,
and local simulation result model. Until those rules and deterministic tests
exist, measurements remain observable output only and cannot drive execution.
The same rule applies to `else`, loops, and stateful branch bodies.

## Non-goals for this profile

This profile does not schedule provider integration, quantum hardware access,
noise modeling, optimization, cloud execution, general foreign-function calls,
an IDE, a package manager, stable language/ABI compatibility, signed release
artifacts, or enterprise support. Each would be an independent product and
security/support commitment.

## Immediate implementation order

1. **U1 Boolean declaration evaluation is complete** as a bounded immutable
   compile-time workflow; it does not add mutable execution semantics.
2. Write the U2 mutable-state contract before accepting assignment syntax.
3. Implement U2 only after its typed state, deterministic ordering, lifetime,
   and rejection rules are testable locally.
4. Implement U3 explicit-parameter quantum routines only after the state and
   resource-effect vocabulary is stable.
5. Implement U4 measurement feedback only after its local and target-side
   storage contract is independently testable.

## References

[1] [OpenQASM live specification: classical instructions](https://openqasm.com/language/classical.html)  
[2] [OpenQASM live specification: subroutines](https://openqasm.com/language/subroutines.html)  
[3] [SynQ project status](./PROJECT_STATUS.md)  
[4] [SynQ tested environments](./TESTED_ENVIRONMENTS.md)
