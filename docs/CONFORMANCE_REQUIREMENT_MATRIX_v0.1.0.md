# SynQ Requirement-to-Test Conformance Matrix v0.1.0

**Matrix status:** Initial evidence matrix for the experimental runtime-subset
candidate. This matrix records what the pinned fixtures exercise; it does **not**
claim that the candidate is a complete runtime profile or that conformance closure
has been achieved.

**Pinned implementation revision:** [`9a1a9b3`][1]

**Remote evidence:** [Compiler Core #32270327206][2] passed 46/46 Linux
recovery-profile CTests, 32/32 Windows MSVC and macOS Clang platform-neutral
CTests, and the three experimental static-SDK clean-install consumer jobs.

> A fixture demonstrates only the requirement rows named here. Passing it does
> not prove unlisted constructs, cross-feature compositions, portability beyond
> its profile, stability, or release readiness.

## Scope and status legend

This matrix covers the currently verified evidence candidates: bounded immutable
evaluation, top-level mutable state, U5/U6 local classical callables, U3 static
routine expansion, U4 feedback, bounded local probability simulation, supported
CLI behavior, and experimental ABI boundaries. The public fixture index gives the
source downloads, exact CTest names, and platform labels.[3]

| Status | Meaning |
| --- | --- |
| **Verified evidence** | A focused fixture and its owning profile are included in the pinned completed remote run. |
| **Partial evidence** | The feature has focused tests, but the wider semantic/profile closure required for a named runtime claim is not yet defined or tested. |
| **Gap** | The claim standard requires this evidence, but the current recovery profile does not provide it. |

## Requirement-to-test mapping

| Requirement ID | Requirement and exact boundary | Evidence class | Primary fixture IDs and CTest owners | Profile scope | Status |
| --- | --- | --- | --- | --- | --- |
| `REQ-FOUND-001` | The recovery compiler parses, lowers, and links the core typed path for its documented bounded subset. | Positive/regression smoke. | `CF-CORE-001` · `synq_core_smoke` | Platform-neutral. | Verified evidence |
| `REQ-CLASS-001` | Opt-in immutable bounded evaluation returns deterministic values or structured budget/overflow failures; it is not target-side execution. | Positive, negative, and limit evidence. | `CF-CLASSICAL-001` · `synq_bounded_evaluator_smoke` | Platform-neutral. | Partial evidence |
| `REQ-STATE-001` | Feature-gated top-level typed state evaluates in source order within stated cell, transition, depth, and operation limits; output paths reject state where documented. | Positive, negative, and limit evidence. | `CF-STATE-001` · `synq_bounded_state_evaluator_smoke` | Platform-neutral. | Partial evidence |
| `LCEP-COMP-001` | The existing constants, state, and runtime evaluator modes agree on a shared immutable subset and atomically reject one another’s unsupported state/callable, callable-only, mixed state/callable, and quantum node shapes. | Positive and negative profile-composition smoke. | `synq_local_classical_evaluation_composition_smoke` | Platform-neutral core profile plus ordinary Linux and isolated sanitizer profiles. | Verified evidence in [Compiler Core #32485055898](https://github.com/TangoSplicer/SynQ/actions/runs/32485055898); narrow separation only |
| `LCEP-COMP-002` | Existing immutable Integer/Boolean/String values, typed mutable cells, and U5/U6 typed callable results retain their documented evaluator-mode acceptance and rejection boundaries. | Positive typed-value and negative mode-separation smoke. | `synq_local_classical_evaluation_value_composition_smoke` | Platform-neutral core profile plus ordinary Linux and isolated sanitizer profiles. | Verified evidence in [Compiler Core #32496656921](https://github.com/TangoSplicer/SynQ/actions/runs/32496656921); narrow value-diversity evidence only |
| `LCEP-COMP-003` | The existing local evaluators atomically reject every remaining parser-resolvable quantum/control dispatch category: qubit declaration, direct gate, standalone measurement, control body variants, and parameterized quantum routine declaration/call source. | Negative full-dispatch-matrix smoke after a preceding immutable declaration. | `synq_local_classical_evaluation_full_matrix_smoke` | Platform-neutral core profile plus ordinary Linux and isolated sanitizer profiles. | Verified evidence in [Compiler Core #32533581417](https://github.com/TangoSplicer/SynQ/actions/runs/32533581417); complete current dispatch-category matrix only |
| `REQ-CALL-U5-001` | U5 accepts only the one-formal local callable slice, preserves exact types and earlier definitions, and rejects unsupported use before target/simulator/ABI execution. | Positive, negative, limit, and cross-path boundary evidence. | `CF-CALL-U5-001` · `synq_classical_callable_runtime_smoke` | Platform-neutral. | Partial evidence |
| `REQ-CALL-U6-001` | U6 accepts only two ordered same-type Integer/Boolean formals with no capture; execution is opt-in, bounded, atomic on failure, and rejected by non-runtime paths. | Positive, negative, limit, overflow, and cross-path boundary evidence. | `CF-CALL-U6-001` · `synq_binary_classical_callable_runtime_smoke`; `CF-ABI-C-001` · `synq_c_abi_smoke` | Platform-neutral. | Partial evidence |
| `REQ-QUANTUM-U3-001` | U3 accepts only its documented parameterized routine signatures and static strict-Hybrid expansion shapes; simulator and ABI execution remain rejected. | Positive, negative, limit, and output-boundary evidence. | `CF-QUANTUM-U3-001` · `synq_parameterized_routine_smoke` | Platform-neutral. | Partial evidence |
| `REQ-HYBRID-U4-001` | U4 accepts one named measurement followed immediately by one direct same-result `x` correction, with a terminal lifetime and deterministic two-branch local model. | Positive, negative, source-export, and simulator-boundary evidence. | `CF-HYBRID-U4-001` · `synq_measurement_feedback_smoke`; `CF-OPENQASM-U4-001` · `synq_measurement_feedback_hybrid_openqasm_export` | Smoke: platform-neutral. Reference parsing: Ubuntu full profile. | Partial evidence |
| `REQ-SIM-001` | The local simulator computes documented bounded ideal-state probabilities and rejects unsupported resource and runtime-node shapes. | Positive, negative, and bound evidence. | `CF-SIM-001` · `synq_bounded_simulator_smoke` | Platform-neutral. | Partial evidence |
| `REQ-CLI-001` | The recovery CLI supports its documented validate/export/inspect/evaluate/simulate commands and reports bounded failures. | End-to-end regression smoke. | `CF-CLI-001` · `synq_cli_smoke` | Platform-neutral when enabled. | Verified evidence |
| `REQ-ABI-001` | ABI v1 owns/frees documented outputs and rejects unsupported U5/U6 local-runtime execution rather than silently exporting or executing it. | Positive and explicit rejection evidence. | `CF-ABI-C-001` · `synq_c_abi_smoke` | Platform-neutral C ABI smoke. | Verified evidence for the experimental boundary only |
| `REQ-INTEROP-001` | Direct Rust, Common Lisp, Clojure, and Mercury fixtures can consume the experimental opaque C ABI under their documented test setup. | Consumer regression smoke. | `CF-ABI-RUST-001`, `CF-ABI-LISP-001`, `CF-ABI-CLOJURE-001`, `CF-ABI-MERCURY-001` | Ubuntu full profile only. | Verified evidence for test consumers only |

## Closure gaps that this matrix exposes

| Closure requirement from the runtime standard | Current matrix finding | Required next evidence |
| --- | --- | --- |
| Versioned semantic-profile specification | A draft named profile now records the three mutually exclusive local evaluator modes, values, scope, budgets, and exclusions, but it is not yet an accepted closed grammar/effect specification. | Convert the draft into a normative profile with requirement IDs and executable evidence for every supported and rejected construct. |
| Cross-feature execution closure | U1/U2/U5/U6 have focused local meanings, but the matrix does not define or test all state/callable/feedback compositions. | Explicit composition contract and positive/negative cross-feature fixtures. |
| Hybrid closure | U4 covers one terminal pair only; repeated results, branch bodies, `else`, and callable/result composition are unrepresented. | Independently specified bounded hybrid-control slice with local and source-representation tests. |
| Differential evidence | No current fixture compares SynQ behavior against a second independently implemented semantic oracle for the same accepted profile. | A carefully scoped reference model or differential oracle, where technically meaningful. |
| Fuzzing, sanitizers, and replay | Compiler Core #32369872013 adds a 32-test Linux/Clang ASan/UBSan core profile; there is still no parser/runtime fuzz/property target or replay protocol. | Versioned fuzz/property targets, deterministic replay controls, corpus handling, and evidence. |
| Release closure | No compatibility/deprecation policy, reproducible-build proof, known-limit ledger, release checklist, or two consecutive unchanged matrices exists. | Versioned release-candidate process and repeated six-job results. |

## Matrix maintenance rule

The next matrix version must be created whenever the named profile, a requirement
ID, a fixture owner, a platform scope, or the remote evidence run changes. It
must record added/removed requirements, added/removed fixtures, changed profile
coverage, and unresolved gaps. It must never infer coverage from a raw CTest
count or mark a requirement closed solely because a source file exists.[4]

## References

[1]: https://github.com/TangoSplicer/SynQ/tree/9a1a9b3 "SynQ implementation revision 9a1a9b3"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/32270327206 "Compiler Core #32270327206 — verified U6 implementation"
[3]: ./CONFORMANCE_FIXTURE_MANIFEST_v0.1.0.md "SynQ Conformance Fixture Manifest v0.1.0"
[4]: ./RUNTIME_SUBSET_CLAIM_STANDARD.md "SynQ Runtime Subset Claim Standard"
