# SynQ Runtime-Subset Claim Standard

**Status:** Proposed acceptance standard. This document does **not** claim that
SynQ already has a complete runtime, a stable release, a general-purpose
language implementation, or a hardware service.

**Current implementation baseline:** The remotely verified U5 callable revision
`7307c00` passed [Compiler Core #32266056516][1]: 45/45 Linux recovery CTests,
31/31 Windows MSVC and macOS Clang platform-neutral CTests, and three
clean-install experimental static-SDK consumer jobs.

## Purpose

SynQ may use the term **complete runtime for a named subset** only when every
construct in that published subset has one of two outcomes: a deterministic,
documented local execution meaning or a source-location-aware rejection before
execution. This is intentionally narrower than a claim of universal language
completeness.

> **Permitted future claim:** “SynQ provides a complete local runtime for the
> documented `<edition>/<subset>` profile.”

The claim must name the profile version, supported source constructs, resource
limits, supported platforms, verification run, and exclusions. It must not be
shortened to “SynQ is a complete runtime” in release, website, package, or
marketing material.

## Non-negotiable acceptance gates

| Gate | Required evidence | Explicitly insufficient evidence |
| --- | --- | --- |
| **Specification closure** | A versioned normative subset specification defines grammar, static types, values, scope, evaluation order, callable/quantum resource effects, errors, and resource budgets. | Parser acceptance, examples, or implementation comments alone. |
| **Execution closure** | Every supported construct executes through the same typed representation used by validation, with deterministic output or deterministic structured failure. | Strict Hybrid source export alone. |
| **Hybrid closure** | Measurement-result lifetime, correction/branch rules, simulator state transitions, and target-source lowering are separately specified and tested. | Valid OpenQASM syntax or a circuit diagram alone. |
| **Diagnostic closure** | Every rejected source shape has a stable diagnostic category, source provenance, and beginner-readable remediation. | A crash, assertion, silently ignored node, or backend-dependent error. |
| **Conformance closure** | A checked requirement-to-test matrix includes positive, negative, regression, cross-feature, and limit fixtures. | A raw test count or a single successful example. |
| **Reliability closure** | Bounded fuzz/property tests, applicable sanitizer runs, deterministic replay, and documented known limits are executed. | A clean normal build only. |
| **Release closure** | Rebuild/install/test instructions, compatibility and deprecation rules, reproducible-build evidence, versioned examples, and a release checklist exist. | A source snapshot or an unversioned CI artifact. |

Conformance testing is limited by the specification: a behavior cannot be
tested for conformance until it is defined. Passing a suite increases confidence
but cannot prove correctness in untested areas.[2] SynQ therefore publishes
both positive claims and exclusions.

## Zero-cost operating rule

The runtime subset must remain buildable, testable, and learnable without a paid
provider, purchased hardware, proprietary compiler, mandatory cloud service, or
custom-domain dependency. Required paths use the repository, CMake/CTest,
ordinary native toolchains, and free CI within their available quotas. This is a
design constraint, not a promise that external free-tier quotas will never
change.

No runtime feature may require quantum-hardware credentials, a provider account,
or an AI API key. Such integrations, if ever pursued, are separate opt-in
services with their own security, consent, cost-control, and operational
contracts.

## Learnability acceptance rule

Every runtime feature must ship with the following four assets before it is
called available:

1. One minimal, copyable, runnable example.
2. One plain-language explanation of values, effects, and output.
3. One deliberately invalid example with its expected diagnostic and recovery
   action.
4. One statement of what the feature does **not** do.

The source vocabulary must preserve progressive disclosure: simple classical and
quantum statements remain readable without implicit host execution, hidden
allocation, hidden hardware submission, or implicit AI action.

## Differentiated hybrid-design rule

SynQ’s distinctive value is not a generic claim to outperform established
languages or SDKs. Each runtime increment must make one **inspectable hybrid
guarantee** that is testable in source, IR, execution, and export:

| Current or future guarantee | Required proof |
| --- | --- |
| Classical/quantum boundary is visible in source | Parser/AST fixture and beginner example. |
| Quantum resource use is explicit | Resolver range/alias/resource-negative fixtures. |
| Feedback has traceable provenance | Result lifetime, branch/correction, and exporter/simulator fixtures. |
| Experimental behavior is opt-in | Feature-gate parsing, diagnostics, and compatibility fixtures. |
| Transformations are inspectable | Semantic inspection or deterministic IR/export output with a regression fixture. |

This makes SynQ useful for developers who need to learn and review hybrid
behavior, while avoiding fabricated claims of uniqueness, performance
superiority, hardware reach, or general compatibility.

## Security acceptance rule

Each new executable form must define and test all applicable controls below.

| Control | Requirement |
| --- | --- |
| **Limits** | Maximum source size, expression depth, operations, call depth, qubits, branches, memory, and execution steps are bounded and diagnostic on exhaustion. |
| **Determinism** | Runtime results, error selection, and replay behavior are deterministic for the documented local model; any seed is observable and replayable. |
| **Input hardening** | Parser and runtime fuzz/property targets accept malformed input without crashing, leaking, or silently changing meaning. |
| **Native boundary** | The C ABI keeps documented ownership and rejects unsupported executable nodes rather than accidentally executing them. |
| **Feature isolation** | Alpha behavior remains disabled by default and requires explicit source opt-in. |
| **No ambient authority** | The runtime does not read secrets, access networks, submit hardware jobs, invoke AI services, or write outside documented local outputs without a separate explicit contract. |

LLVM documents a coverage-guided in-process fuzzing model and notes that current
Clang distributions include the associated tooling; bounded local or CI fuzz
runs may therefore provide no-cost additional evidence.[3]

## Runtime-subset graduation sequence

1. **Versioned semantic core:** values, lexical scopes, deterministic evaluation,
   assignments, structured runtime errors, and budgets.
2. **Bounded callable runtime:** U5 completes one non-recursive, one-formal,
   parameter-only local callable slice with a depth-1 frame. Wider arguments,
   returns, scopes, and effects remain separate contracts.
3. **Bounded hybrid runtime:** branch semantics, repeated measurement/result
   lifetime rules, explicit correction effects, and a deterministic simulator
   model; source lowering remains separately described.
4. **Conformance and hardening:** requirement-to-test matrix, regression corpus,
   fuzz/property targets, applicable sanitizers, deterministic replay, and
   updated interoperability boundaries.
5. **Release candidate:** reproducible rebuild/install path, compatibility
   policy, tutorials, known-limit ledger, and at least two consecutive clean
   six-job Compiler Core matrices for an unchanged runtime profile.

Only after all five stages may the named runtime-subset claim be published.
Hardware service remains outside this standard.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32266056516 "SynQ Compiler Core — Alpha bounded classical callable runtime"
[2]: https://www.nist.gov/itl/voting/conformance-testing-101 "NIST Conformance Testing 101"
[3]: https://llvm.org/docs/LibFuzzer.html "LLVM libFuzzer documentation"
[4]: https://reproducible-builds.org/ "Reproducible Builds"
