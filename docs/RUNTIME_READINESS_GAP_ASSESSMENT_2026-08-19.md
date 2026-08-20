# SynQ Runtime Readiness Gap Assessment — 19 August 2026

**Assessment status:** Evidence-based roadmap note, not a release announcement.

## Short answer

SynQ has a **verified bounded execution foundation**, not a complete runtime.
Compiler Core run [#32374149046][1] verified revision `8fc1de5` with 47/47
ordinary Linux recovery CTests, 33/33 Windows MSVC and macOS Clang
platform-neutral CTests, three static-SDK consumer jobs, and a separate 33/33
Linux/Clang ASan/UBSan core profile. The same run includes a fixed-corpus
deterministic parser/lowering/resolution/bounded-evaluation replay target. That
evidence covers bounded immutable and mutable classical evaluation, narrow U5/U6
local callable frames, constrained quantum source expansion/simulation, and one
exact U4 measurement-feedback pair.[2] [3]

> The nearest truthful milestone is **“complete local runtime for a named,
> versioned subset”**. It is not “SynQ is a complete runtime,” a
> general-purpose language claim, or a hardware-service claim.[2]

## What is already present

| Area | Verified bounded evidence | Why it is not sufficient alone |
| --- | --- | --- |
| Classical execution | Immutable evaluation, bounded top-level state, U5 one-formal calls, and U6 same-type two-formal calls run only through explicit local opt-ins and fixed budgets. | No versioned overall value/scope/effect specification or general lexical runtime exists. |
| Quantum and hybrid work | Bounded local probability simulation, source-only U3 routine expansion, and one terminal U4 measurement/direct-`x` feedback model. | Feedback does not become a general runtime value and does not compose with general local control. |
| Diagnostics and boundaries | Source-aware parser/resolver/evaluator diagnostics, feature gates, limits, and exporter/simulator/ABI rejection paths are tested for selected shapes. | The project lacks a defined diagnostic-compatibility policy and a requirement-to-test closure matrix. |
| Evidence and interoperability | Seven-job Compiler Core evidence, including an isolated Linux/Clang sanitizer profile, a fixed deterministic replay corpus/schedule, experimental C ABI fixtures, and a curated pinned source-fixture manifest. | The replay target is not continuous fuzzing or a complete conformance suite; neither it nor the sanitizer profile creates a stability policy, package ecosystem, or support commitment. |

## Remaining work before a named complete-runtime claim

| Priority | Remaining gate | Concrete completion condition | Current state |
| --- | --- | --- |
| 1 | **Freeze a named profile** | Publish a normative profile version that lists every accepted construct and every rejection, values, no-capture scope rules, evaluation order, error categories, limits, and exclusions. | Not complete. Existing contracts are feature-level, not one closed runtime profile. |
| 2 | **Close execution semantics** | Make every construct in that profile execute through validated typed IR or reject before execution with source location; define atomic failure and state interaction across features. | Partial. U1/U2/U5/U6 have local meanings, but their cross-feature composition and full profile semantics are not closed. |
| 3 | **Close hybrid semantics** | Define the lifetime and representation of measurement results, branch/correction effects, local simulator transitions, and source lowering for the exact chosen profile. | Partial. U4 verifies one pair, but there is no repeated feedback, `else`, branch body, or callable/result integration contract. |
| 4 | **Create conformance closure** | Publish a requirement-to-test matrix with positive, negative, limit, regression, and cross-feature fixtures; keep it in free CI. | Not complete. The v0.1.0 manifest is a useful entry index, not the required complete matrix. |
| 5 | **Add reliability closure** | Run applicable sanitizers, parser/runtime fuzz or property tests, deterministic replay, and document known limits. | Partial. A Linux/Clang 33-test ASan/UBSan profile, fixed deterministic replay corpus/schedule, and versioned known-limit ledger passed or published with run #32374149046. Continuous fuzzing/property coverage, a larger corpus, and a formal security review remain absent. |
| 6 | **Create release closure** | Prove reproducible rebuild/install, define compatibility/deprecation policy, version examples and diagnostics, maintain a release checklist and known-limit ledger, then pass two consecutive unchanged seven-job matrices. | Not complete. The build/install path is experimental; release and compatibility evidence are not yet closed. |

## What can remain out of scope

SynQ does **not** need arbitrary arity, recursion, general `else`, loops,
hardware access, stable ABI, packages, or provider integration to claim a
complete runtime for a carefully restricted named profile. Those features may be
explicitly excluded if the profile rejects them deterministically before local
execution. They are required only for a broader general-purpose or hardware
product claim.[2] [3]

## Recommended order

The lowest-risk path is to freeze a small **local runtime profile** around the
already tested U1/U2/U5/U6 semantics, explicitly decide whether the U4 feedback
pair belongs in that profile, and make every unsupported composition reject
before execution. Next, build the requirement-to-test matrix and hardening
evidence; only then prepare the repeatable release-candidate evidence. Wider
callables and general hybrid control should be separate increments, not assumed
prerequisites for the narrow claim.

## Separate future claims

| Claim | Additional work beyond a named local runtime |
| --- | --- |
| **General runtime** | General values, lexical scopes, returns, effects, arbitrary call shapes, control flow, measurement values, routine execution, and a larger semantic and compatibility commitment. |
| **Stable SDK or broad interoperability** | Frozen ABI policy, versioned safe wrappers/packages, artifact lifecycle, ownership/error conformance, and platform commitments. |
| **Hardware service** | Provider architecture, credentials and consent, security/threat model, cost controls, queue/error model, provider integration tests, and honest provider support matrix. |

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32374149046 "Compiler Core #32374149046 — verified deterministic replay and additive Linux Clang ASan/UBSan profile"
[2]: ./RUNTIME_SUBSET_CLAIM_STANDARD.md "SynQ Runtime Subset Claim Standard"
[3]: ./OPERATIONAL_READINESS.md "SynQ Operational Readiness Definition"
