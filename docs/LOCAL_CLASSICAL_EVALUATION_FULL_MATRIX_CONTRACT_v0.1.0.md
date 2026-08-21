# SynQ Local Classical Evaluation Full Matrix Contract v0.1.0

**Status:** Remotely verified fixture contract. This is a completeness claim only for the **current parser-resolvable
top-level evaluator-dispatch categories** listed below. It does not claim all
program permutations, diagnostic completeness, language completeness, or a
complete runtime.

**Requirement family:** `LCEP-COMP-003`, following the verified narrow
separation and value-diversity requirements `LCEP-COMP-001` and
`LCEP-COMP-002`.

## Matrix scope

The three local modes dispatch over resolved top-level nodes. Existing fixtures
already cover the positive classical categories and their direct cross-mode
rejections. This contract covers the remaining parser-resolvable quantum and
control categories independently, with one preceding immutable declaration in
each source so rejection must be atomic rather than return a partial local
result.

| Top-level category | Existing acceptance evidence | Constants outcome | State outcome | Runtime outcome |
| --- | --- | --- | --- | --- |
| Immutable declaration | `LCEP-COMP-001`, `LCEP-COMP-002` | Accept | Accept without cells | Accept |
| Mutable declaration and assignment | `LCEP-COMP-001`, `LCEP-COMP-002` | `SYNQ-E002`, no result | Accept | `SYNQ-E014`, no result |
| U5/U6 local callable declaration and invocation | `LCEP-COMP-001`, `LCEP-COMP-002` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | Accept |
| Qubit declaration | `LCEP-COMP-003A` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |
| Direct quantum gate | `LCEP-COMP-003B` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |
| Standalone named measurement | `LCEP-COMP-003C` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |
| Control with quantum-gate body | `LCEP-COMP-003D` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |
| Control with measurement body | `LCEP-COMP-003E` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |
| Parameterized quantum routine declaration and call source | `LCEP-COMP-003F` | `SYNQ-E002`, no result | `SYNQ-E010`, no result | `SYNQ-E014`, no result |

The matrix treats a source containing a parameterized routine declaration and
call as one evaluator input. The local evaluators reject at the earlier nonlocal
routine declaration, which is intentional: this fixture does not claim that an
otherwise unreachable routine-call node has been locally executed or
individually diagnosed.

## Invariants

`LCEP-COMP-003` requires exactly one top-level diagnostic and no evaluation
object from every rejecting local mode. It does not inspect an internal partial
map, lower a target, simulate a program, exercise an ABI call, or execute
quantum/control semantics. The established parser, lowering, resolver,
exporter, and simulator tests remain responsible for their own positive
behavior.

## Test ownership and promotion rule

`compiler/tests/smoke/local_classical_evaluation_full_matrix_smoke.cpp` owns
the fixture and is registered as
`synq_local_classical_evaluation_full_matrix_smoke`. It belongs to the ordinary
core-smoke profile and is intended for ordinary Linux, Windows, macOS, and the
isolated Linux/Clang sanitizer profile.

The full local recovery suite passed 51/51 and the isolated local Linux/Clang
sanitizer profile passed 36/36 for this implementation. The exact revision then
passed all seven jobs in [Compiler Core
#32533581417](https://github.com/TangoSplicer/SynQ/actions/runs/32533581417):
51/51 ordinary Linux CTests, 37/37 Windows and macOS CTests, three SDK-consumer
jobs, and 36/36 sanitizer CTests. This establishes a complete **current
dispatch-category matrix**, not a unified evaluator, runtime, stable API,
hardware service, or security claim.

## References

[1]: ./LOCAL_CLASSICAL_EVALUATION_PROFILE_DRAFT_v0.1.0.md "SynQ Local Classical Evaluation Profile draft"
[2]: ./LOCAL_CLASSICAL_EVALUATION_COMPOSITION_CONTRACT_v0.1.0.md "LCEP-COMP-001 contract"
[3]: ./LOCAL_CLASSICAL_EVALUATION_VALUE_COMPOSITION_CONTRACT_v0.1.0.md "LCEP-COMP-002 contract"
[4]: ./BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md "SynQ parameterized routine contract"
[5]: ./BOUNDED_MEASUREMENT_FEEDBACK.md "SynQ measurement feedback contract"
