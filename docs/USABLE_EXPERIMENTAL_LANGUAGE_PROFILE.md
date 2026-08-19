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
| Classical | Immutable top-level Boolean, Integer, and String declarations; bounded Boolean and one-operation Integer expression trees; recursively evaluated Boolean declaration expressions; and feature-gated top-level typed `var`/`set` cells evaluated deterministically under explicit budgets. | No scopes, precedence grammar, loop/branch execution, target-side state lowering, measurement values, or general runtime semantics. |
| Quantum | Typed gates, explicit declared-register operands, named-register allocation for bounded ideal-probability simulation, measurements, and strict OpenQASM 3 source export. | No noise, sampling/collapse, reset lifecycle, hardware/provider execution, optimization, dynamic allocation, or general resource ownership. |
| Callable composition | A zero-parameter one-gate kernel remains available. U3 additionally supports exactly three parameterized kernels: literal angle plus qubit rotation/phase, one qubit fixed gate, or two qubit `cx`; strict Hybrid export expands one source-order gate per verified call. | No returns, function bodies, capture, allocation, recursion, nested routine calls, runtime angle/execution, routine simulation, or ABI execution. |
| Hybrid control | Literal and bounded immutable-Boolean `if` source-lowering forms with one gate body, plus U4 one-result/one-direct-`x` feedback through exact strict source lowering and deterministic local branch enumeration. | No target-side user state writes, `else`, loops, general branch execution, multiple results, general measurement APIs, or general target-control contract. |

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
| U2 — Explicit classical state | **Completed Alpha local-only slice:** typed top-level mutable cells, exact-type whole-cell assignment, declaration/write lifetime provenance, and deterministic `--eval-state` evaluation with explicit resource limits, as specified in [`BOUNDED_MUTABLE_CLASSICAL_STATE.md`](./BOUNDED_MUTABLE_CLASSICAL_STATE.md). | Quantum operations retain explicit resource validation; state nodes are rejected by simulation. | No measurement feedback, loop/branch execution, target-side storage, or target-side assignment lowering. | Parser/IR/resolver/evaluator/CLI/semantic/exporter/simulator fixtures plus the six-job remote matrix passed in [Compiler Core #32242711770](https://github.com/TangoSplicer/SynQ/actions/runs/32242711770). |
| U3 — Practical quantum routines | **Completed Alpha slice:** typed angle/qubit formals, three one-gate signatures, earlier-only lookup, ordered actual validation, duplicate-qubit alias rejection, bounded strict-Hybrid source expansion, and explicit simulator/C-ABI rejection, as specified in [`BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md`](./BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md). | Reusable routines over caller-supplied explicit qubits; no allocation inside a routine. | Angles remain documented literals and expansion remains source generation, not execution. | Parser/AST/IR/resolver/exporter/CLI/reference-parser/simulator/ABI fixtures plus the six-job remote matrix passed in [Compiler Core #32247154982](https://github.com/TangoSplicer/SynQ/actions/runs/32247154982). |
| U4 — Measurement feedback | **Completed Alpha slice:** one named measurement result is immediately consumed by one direct conditional `x` correction; resolver preserves result provenance and enforces direct single use and terminal lifetime. | One measurement can drive one in-range explicit-qubit `x` correction, including the same qubit. | Strict Hybrid uses one exporter-owned scalar bit plus one measurement assignment/conditional gate; local simulation deterministically enumerates the two branches and does not expose a sampled host value. | Parser/AST/IR/resolver/exporter/CLI/reference-parser/simulator/ABI fixtures plus the six-job remote matrix passed in [Compiler Core #32250265354](https://github.com/TangoSplicer/SynQ/actions/runs/32250265354). |

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

U4 supplies only one evidence-bounded feedback meaning: one named measurement,
one immediate direct `x` correction, one target scalar bit in emitted source, and
two deterministic local probability branches. It does not make the result a
general Boolean value or sample it into a host value. The same strict boundary
continues to apply to `else`, loops, multiple results, and stateful branch bodies.

## Non-goals for this profile

This profile does not schedule provider integration, quantum hardware access,
noise modeling, optimization, cloud execution, general foreign-function calls,
an IDE, a package manager, stable language/ABI compatibility, signed release
artifacts, or enterprise support. Each would be an independent product and
security/support commitment.

## Immediate implementation order

1. **U1 and U2 are complete** as bounded immutable evaluation and bounded
   local-only typed-state workflows; neither adds target-side execution semantics.
2. **U3 is complete** as a bounded source-expansion-only routine layer; it does
   not add routine execution or a general callable model.
3. **U4 is complete** as one bounded feedback pair; it does not add general
   measurement control or a general hybrid runtime.

## References

[1] [OpenQASM live specification: classical instructions](https://openqasm.com/language/classical.html)  
[2] [OpenQASM live specification: subroutines](https://openqasm.com/language/subroutines.html)  
[3] [SynQ project status](./PROJECT_STATUS.md)  
[4] [SynQ tested environments](./TESTED_ENVIRONMENTS.md)
