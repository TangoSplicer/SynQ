# Local Classical Evaluation Composition Contract v0.1.0

**Status:** Implemented test contract for the draft Local Classical Evaluation
Profile. It adds profile-level evidence for existing evaluator-mode separation;
it does **not** add a language feature, merge evaluator modes, close conformance,
or justify a complete-runtime claim.

**Requirement family:** `LCEP-COMP-001` from
[`LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md`](./LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md).

## Purpose

SynQ currently exposes three local evaluator modes: immutable constants, mutable
state, and U5/U6 callable runtime. Each mode has focused tests. This contract
adds one profile-level smoke fixture to prove that the modes agree on their
shared immutable subset and reject one another’s executable forms atomically.

| Test ID | Source shape | Required outcome |
| --- | --- | --- |
| `LCEP-COMP-001A` | Immutable `let` declarations using the existing bounded Integer expression subset. | All three modes return the same ordered immutable values. |
| `LCEP-COMP-001B` | Top-level `var` plus whole-cell `set`. | `state` returns final cell state; `constants` rejects with `SYNQ-E002`; `runtime` rejects with `SYNQ-E014`; rejected modes return no evaluation. |
| `LCEP-COMP-001C` | Earlier U5 and U6 callable declarations followed by immutable invocation results. | `runtime` returns the ordered results; `constants` rejects with `SYNQ-E002`; `state` rejects with `SYNQ-E010`; rejected modes return no evaluation. |
| `LCEP-COMP-001D` | One source containing state and a U5 callable invocation. | The parser/lowerer/resolver may prepare the source, but all three evaluator modes reject it atomically because no combined state/callable evaluator exists. |
| `LCEP-COMP-001E` | Immutable declaration followed by a declared qubit and one supported gate. | All three local evaluators reject the quantum node atomically; no evaluator executes or exports a hybrid program. |

## Non-goals

The fixture does not make mutable state callable-visible, permit functions to
mutate cells, add returns or nested calls, allow measurement-derived values,
execute branches, execute quantum operations, invoke providers, or expose
evaluation through ABI v1. Existing feature gates and parser/resolver rules
remain authoritative.

## Test ownership and evidence boundary

`compiler/tests/smoke/local_classical_evaluation_composition_smoke.cpp` owns the
fixture and is registered as `synq_local_classical_evaluation_composition_smoke`.
It uses the ordinary core-smoke registration, so it enters the ordinary Linux,
Windows, macOS, and additive sanitizer configurations when those profiles are
run. A local pass is only local evidence. The requirement is not recorded as
remotely verified until the full local recovery suite and the seven-job Compiler
Core matrix both pass for the implementation revision.
