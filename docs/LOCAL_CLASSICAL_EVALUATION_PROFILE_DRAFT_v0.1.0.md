# SynQ Local Classical Evaluation Profile — Draft v0.1.0

**Status:** Draft semantic-profile contract. It organizes currently implemented
bounded evaluator modes into one reviewable vocabulary, but it does **not**
declare a complete runtime, a stable language profile, conformance closure, a
hardware service, or a new verified language capability.

**Implementation evidence used by this draft:** the bounded evaluator entry
points and option defaults in `compiler/src/compiler/bounded_evaluator.{h,cpp}`;
the existing U1–U6 contracts; and exact-source Compiler Core
[#32403435066][1]. The release tag `v0.1.0-experimental` points to the tested
source commit `812528711dc9189ba7d1af27657821b9d54a39ab`. This draft records the
implemented behavior at that boundary; it does not make the later documentation
commit part of the tested compiler claim.

## 1. Profile identity and claim boundary

The proposed profile name is **SynQ Local Classical Evaluation Profile
v0.1.0-draft**. It is a shared name for three explicit, mutually exclusive local
evaluation modes:

| Mode | CLI entry point | Purpose | Profile status |
| --- | --- | --- | --- |
| `constants` | `synqc --eval-constants` | Evaluate bounded immutable declarations. | Implemented bounded mode; profile closure pending. |
| `state` | `synqc --eval-state` | Evaluate top-level mutable cells and whole-cell assignments. | Implemented bounded mode; profile closure pending. |
| `runtime` | `synqc --eval-runtime` | Evaluate immutable declarations and documented U5/U6 local callable invocations. | Implemented bounded mode; profile closure pending. |

Each command uses an explicit local opt-in and produces local values or
diagnostics only. A source program is evaluated in **one mode at a time**. The
modes are not a unified evaluator and do not establish general interoperability
or composition among immutable evaluation, state, callable execution, quantum
execution, feedback, target lowering, simulation, or the C ABI.

> **Forbidden shorthand:** Until every gate in
> [`RUNTIME_SUBSET_CLAIM_STANDARD.md`](./RUNTIME_SUBSET_CLAIM_STANDARD.md) is
> closed, neither this document nor any release, website, package, or marketing
> material may say that “SynQ has a complete runtime.”

## 2. Shared value model

The current evaluator stores one of three tagged values. No implicit
coercions, collections, floating-point values, qubit values, measurement-result
values, user-defined records, functions as values, or null-like value exists in
this profile.

| Static value | Current representation and meaning | Explicit exclusions |
| --- | --- | --- |
| `Integer` | Signed `int64` value. Supported arithmetic is limited to the expression forms admitted by the applicable mode. Checked arithmetic rejects invalid or overflowing results. | Arbitrary precision, decimal/floating values, implicit numeric conversion, division, remainder, exponentiation, and general expressions. |
| `Boolean` | `true` or `false`. Supported Boolean operators are only the documented bounded forms. | Truthiness, user-defined predicates, short-circuit runtime meaning, and general conditions. |
| `String` | A quoted literal with quotes removed in the evaluator result. | Concatenation, indexing, interpolation, mutation, encoding transforms, and general string operations. |

All accepted references resolve to an earlier compatible binding in the relevant
mode. A missing or incompatible reference is rejected with a diagnostic rather
than coerced or dynamically looked up.

## 3. Scope, ordering, and evaluator composition

### 3.1 Top-level sequential scope

The profile recognizes one ordered top-level namespace. A declaration becomes
available only after its initializer or supported invocation has completed.
There is no lexical block scope, shadowing, closure capture, dynamic scope,
module import scope, implicit global mutation, or forward reference.

`state` records mutable-cell identity separately from immutable bindings, and
only an earlier active mutable cell may be the target of `set`. `runtime` builds
a fresh, non-capturing frame for a callable invocation; U5 frames contain one
formal and U6 frames contain two ordered same-type formals. Call frames cannot
read the caller’s other bindings, mutate top-level cells, declare values, return
functions, or persist after evaluation.

### 3.2 Source-order evaluation

Each mode traverses resolved nodes from first to last. It evaluates a supported
initializer or invocation using only previously materialized bindings, then
adds the resulting binding. Callable declarations in `runtime` are registered
only for later invocation. `state` updates a whole cell only after its
right-hand side has been successfully evaluated and exact static-type agreement
is confirmed.

When a mode fails, it returns diagnostics and no successful profile result. This
is externally atomic: partial local maps, frames, or cell updates are not
reported as an evaluation result. It does not promise transactional semantics
for any future persistent store, target, ABI caller, provider, or hardware
execution path.

### 3.3 Intentional mode separation

| Source form | `constants` | `state` | `runtime` |
| --- | --- | --- | --- |
| Immutable `let` with supported expression | Accepted. | Accepted. | Accepted. |
| Top-level `var` / whole-cell `set` | Rejected. | Accepted. | Rejected. |
| U5 one-formal local `fn` / invocation | Rejected. | Rejected. | Accepted. |
| U6 two-formal same-type local `fn` / invocation | Rejected. | Rejected. | Accepted. |
| Quantum statement, quantum routine, measurement feedback, `if`, loop, target export node, or C ABI execution | Rejected. | Rejected. | Rejected. |

The profile consequently does **not** specify callable access to mutable state,
stateful callable return or assignment, callable invocation from a branch,
measurement-derived values, feedback-driven classical execution, quantum-target
execution, or a general hybrid control model. Those are future contracts, not
implicit extensions.

## 4. Current executable surfaces and budgets

All limits are explicit caller-configurable options with the defaults below.
Exhaustion is diagnostic; it must not be treated as a silent truncation or a
license to raise defaults in a release claim without separate evidence.

| Limit | `constants` default | `state` default | `runtime` default |
| --- | ---: | ---: | ---: |
| Immutable declaration cap | 64 | Not separately capped by this evaluator | Not separately capped by this evaluator |
| Mutable-cell cap | N/A | 64 | N/A |
| State-transition cap | N/A | 128 | N/A |
| Callable-declaration cap | N/A | N/A | 32 |
| Callable-invocation cap | N/A | N/A | 128 |
| Call depth | N/A | N/A | 1 (non-nested) |
| Expression depth | 16 | 16 | 16 |
| Operation budget | 128 | 128 | 128 |

The profile has no independently implemented total source-size limit, memory
budget, wall-clock timeout, persistent-state bound, recursion support, random
seed, or general step budget. Existing parser and CLI limits remain distinct
from this evaluator contract and must be recorded separately before a named
runtime claim could be accepted.

## 5. Callable behavior in `runtime`

`runtime` accepts only resolver-produced local callable shapes. A U5 callable
has one formal of `Integer`, `Boolean`, or `String`; a U6 callable has two
ordered formals of the same `Integer` or `Boolean` type. An invocation is valid
only after an earlier callable declaration resolves and only if actuals have the
exact required static type.

| Callable value type | Supported current body | Excluded behavior |
| --- | --- | --- |
| U5 `Integer` | Identity or bounded parameter-plus/minus/times Integer-literal expression. | Other parameters, nested calls, capture, mutable locals, arbitrary expressions, recursion, returns, division, and target execution. |
| U5 `Boolean` | Identity or bounded parameter / `not`-parameter expression. | Arbitrary Boolean expressions, capture, nested calls, branches, and target execution. |
| U5 `String` | Parameter identity only. | String operations, concatenation, mutation, capture, and nested calls. |
| U6 `Integer` | Bounded expression over the two formals with documented plus/minus/times forms. | Mixed types, String formals, arbitrary arity, nested calls, capture, mutable locals, recursion, and returns. |
| U6 `Boolean` | Bounded Boolean expression over the two formals with documented `and`/`or` forms. | Mixed types, arbitrary arity, nested calls, capture, branches, mutation, recursion, and returns. |

The evaluator does not export a callable result through strict-Hybrid OpenQASM,
the bounded simulator, or ABI v1. Those paths reject these executable nodes.

## 6. Diagnostic classes

The implementation currently emits structured `SYNQ-E...` diagnostics with
source spans where an applicable source node exists. This draft groups the
current behavior for future profile-level stability work; it does **not** freeze
the individual diagnostic codes or text as a stable public interface.

| Draft category | Current examples | Required recovery action |
| --- | --- | --- |
| Opt-in and shape rejection | `SYNQ-E000`, `SYNQ-E002`, `SYNQ-E008`, `SYNQ-E011`, `SYNQ-E014` | Choose the documented local command and source subset; do not route unsupported nodes through an evaluator. |
| Name, scope, and static-type rejection | `SYNQ-E003`, `SYNQ-E010`, `SYNQ-E013`, `SYNQ-E015` | Use an earlier compatible binding, exact callable shape, or mutable-cell target. |
| Resource-limit rejection | `SYNQ-E001`, `SYNQ-E009`, `SYNQ-E012`, evaluator depth/operation diagnostics | Reduce the program to the configured bound or explicitly select and document a different bound for a controlled test. |
| Value and arithmetic rejection | `SYNQ-E005` and literal/body validation diagnostics | Use an in-range Integer or the documented bounded expression form. |

The planned profile test matrix must pin diagnostic category, source provenance,
and beginner-readable recovery text for every accepted and rejected construct.

## 7. Conformance obligations before acceptance

This draft is not an accepted named runtime profile. Before it can graduate from
draft, the following requirements must be implemented, mapped to tests, and
remotely verified against one frozen profile revision.

| Draft requirement | Missing closure | Minimum evidence to accept |
| --- | --- | --- |
| `LCEP-SPEC-001` | The three modes are documented separately but not yet versioned as a closed grammar/type/effect specification. | Normative accepted/rejected grammar inventory, exact mode-selection rules, and versioned change policy. |
| `LCEP-COMP-001` | A narrow composition/rejection fixture covers shared immutable declarations plus state/callable, callable-only, mixed state/callable, and quantum crossings; it passed the full local recovery suite and [Compiler Core #32485055898](https://github.com/TangoSplicer/SynQ/actions/runs/32485055898). `LCEP-COMP-002` covers the supported value-diversity slice and passed local recovery (50/50), isolated sanitizer (35/35), and [Compiler Core #32496656921](https://github.com/TangoSplicer/SynQ/actions/runs/32496656921). Neither fixture is comprehensive. | Extend the matrix deliberately to every remaining cell in the mode-separation table. |
| `LCEP-DIAG-001` | Current diagnostics are structured but not a frozen category-and-remediation contract. | Stable category mapping, source-span assertions, and beginner-facing invalid examples for every profile family. |
| `LCEP-LIMIT-001` | Individual U1/U2/U5/U6 limits exist; profile-wide resource and source-size boundaries are incomplete. | Exact boundary fixtures for every listed budget and an explicit decision on source-size, memory, and step limits. |
| `LCEP-REL-001` | Fixed deterministic replay and one sanitizer profile exist; continuous fuzz/property evidence does not. | Bounded parser/runtime property or fuzz target with deterministic reproducers, corpus policy, and applicable sanitizer coverage. |
| `LCEP-REL-002` | One experimental CLI pre-release exists, not a stable runtime candidate. | Two consecutive unchanged seven-job Compiler Core matrices, reproducible rebuild/install evidence, compatibility/deprecation policy, and profile-versioned examples. |

## 8. Learnability pack required for acceptance

Each accepted mode must retain one copyable positive example, one plain-language
explanation, one deliberately invalid example with a diagnostic and recovery
action, and one clear non-goal. Existing U5/U6 tutorials are evidence assets for
their narrow callable slices only; they do not satisfy the profile pack for
`constants` and `state`, nor do they close cross-mode examples.

## 9. Current decision

The next implementation work should **not** add U7+ callable arity, scopes,
returns, recursion, general branching, or wider hybrid feedback before this
draft’s mode separation, diagnostics, and limit requirements have an executable
conformance matrix. The smallest evidence-gated implementation increment after
this draft is therefore **LCEP-COMP-001**: a profile-level composition/rejection
matrix and fixtures for the three existing modes, without changing language
semantics.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32403435066 "SynQ Compiler Core #32403435066"
[2]: ./RUNTIME_SUBSET_CLAIM_STANDARD.md "SynQ Runtime Subset Claim Standard"
[3]: ./CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md "SynQ Conformance Requirement Matrix v0.1.0"
[4]: ./BOUNDED_CLASSICAL_CALLABLE_EXECUTION.md "SynQ U5 bounded callable contract"
[5]: ./BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md "SynQ U6 bounded binary callable contract"
[6]: ./BOUNDED_MUTABLE_CLASSICAL_STATE.md "SynQ U2 bounded mutable state contract"
