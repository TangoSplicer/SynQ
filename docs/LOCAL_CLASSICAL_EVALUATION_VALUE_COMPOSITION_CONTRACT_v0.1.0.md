# SynQ Local Classical Evaluation Value-Composition Contract v0.1.0

**Status:** Remotely verified narrow fixture contract. It extends the profile-level evaluator-separation evidence only. It
does **not** add a type, expression, evaluator composition, callable behavior,
or complete-runtime claim.

**Requirement family:** `LCEP-COMP-002`, following the narrow verified
`LCEP-COMP-001` separation fixture in
[`LOCAL_CLASSICAL_EVALUATION_COMPOSITION_CONTRACT_v0.1.0.md`](./LOCAL_CLASSICAL_EVALUATION_COMPOSITION_CONTRACT_v0.1.0.md).

## Purpose

The existing bounded evaluators admit `Integer`, `Boolean`, and `String` values
in deliberately different shapes. This contract checks the remaining
value-diversity gap in the draft Local Classical Evaluation Profile without
inventing a merged evaluator. In particular, `state` intentionally exposes only
its final mutable cells; it does not expose immutable bindings as a second
result collection.

| Test ID | Source shape | Required outcome |
| --- | --- | --- |
| `LCEP-COMP-002A` | Existing immutable `Integer` arithmetic, `Boolean` expression, and `String` declaration subset. | `constants` and `runtime` expose the same ordered typed immutable values; `state` accepts the source and exposes no mutable cells. |
| `LCEP-COMP-002B` | Existing typed `Integer`, `Boolean`, and `String` mutable cells with whole-cell assignments. | `state` exposes the three final typed cells; `constants` rejects atomically with `SYNQ-E002`; `runtime` rejects atomically with `SYNQ-E014`. |
| `LCEP-COMP-002C` | Existing U5 `Integer`, `Boolean`, and `String` callables plus a U6 `Boolean` callable and immutable invocation results. | `runtime` exposes deterministic typed values; `constants` rejects atomically with `SYNQ-E002`; `state` rejects atomically with `SYNQ-E010`. |

## Explicit exclusions

The fixture does not compare unobservable internal state bindings, make
immutable values mutable, permit a callable to read or write a cell, add String
operations, permit U6 String callables, allow mixed-type actuals, introduce
callable nesting or returns, execute a quantum/control node, lower a target, or
expose local evaluation through ABI v1.

## Test ownership and promotion rule

`compiler/tests/smoke/local_classical_evaluation_value_composition_smoke.cpp`
owns the fixture and is registered as
`synq_local_classical_evaluation_value_composition_smoke` in the ordinary core
smoke profile. It is intended to run in ordinary Linux, Windows, macOS, and the
isolated Linux/Clang sanitizer configuration.

The complete local recovery suite passed 50/50 and the isolated local
Linux/Clang sanitizer profile passed 35/35 for this implementation. The exact
revision then passed all seven jobs in [Compiler Core
#32496656921](https://github.com/TangoSplicer/SynQ/actions/runs/32496656921):
50/50 ordinary Linux CTests, 36/36 Windows and macOS CTests, three SDK-consumer
jobs, and 35/35 sanitizer CTests. This is verified narrow value-diversity
evidence only; it remains evidence for existing evaluator separation—not a
unified runtime or profile closure.

## References

[1]: ./LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md "SynQ Local Classical Evaluation Profile draft"
[2]: ./LOCAL_CLASSICAL_EVALUATION_COMPOSITION_CONTRACT_v0.1.0.md "SynQ LCEP-COMP-001 contract"
[3]: ./BOUNDED_RECURSIVE_CONSTANT_EVALUATION.md "SynQ bounded constant evaluation contract"
[4]: ./BOUNDED_MUTABLE_CLASSICAL_STATE.md "SynQ bounded mutable state contract"
[5]: ./BOUNDED_CLASSICAL_CALLABLE_EXECUTION.md "SynQ U5 bounded callable contract"
[6]: ./BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md "SynQ U6 bounded binary callable contract"
