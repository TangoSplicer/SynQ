# SynQ Operational-Readiness Assessment — Updated 17 August 2026

**Assessment status:** Evidence-based technical assessment, not a release
announcement.
**Assessment scope:** The default CMake recovery profile, its documented `synqc`
workflow, the experimental opaque C ABI and static SDK path, the source-only Alpha
Rust wrapper, and the factual public-facing project position.
**Evidence cut-off:** Compiler Core platform-matrix run
[#32242711770][1] for implementation revision `d62b5ed`, with repository
evidence records aligned through the bounded Alpha U2 mutable-state update.

## Executive assessment

SynQ is **not a fully operational programming-language platform**. It is an
**operational experimental language seed**: a narrowly specified and tested
compiler workflow that a developer can build, validate, diagnose, inspect,
export from, evaluate in a bounded classical subset, and simulate in a bounded
quantum subset. The current recovery profile passed **38/38** CTest checks
locally and in the Ubuntu CI profile; independent Windows MSVC and macOS Clang
profiles each passed **27/27** platform-neutral CTest checks. The same remote
matrix passed Ubuntu 22.04, Windows MSVC, and macOS Clang experimental static-
SDK external-consumer jobs.[1] [2] [3]

Six deliberately constrained evidence increments are now implemented and tested:
read-only inspection of resolved top-level classical bindings, declaration-order
multi-register local probability simulation, and a source-only Alpha Rust wrapper
over the opaque C ABI, plus target-side lowering of one `if` gate controlled by
an earlier Boolean-literal declaration, plus `not` over that same immutable
storage mapping. Those points improve inspectability, resource-model evidence,
one ownership-safe host-language entry point, and one explicitly bounded
classical-storage expression mapping, compile-time negated-literal folding, and
feature-gated typed top-level mutable cells with deterministic local-only evaluation.
They do **not** make
SynQ a general runtime, stable SDK, registry package, hardware client, or
cross-platform delivery product.[2] [4] [5] [6]

> **Readiness finding.** SynQ has completed the operational-baseline gates for a
> bounded experimental developer workflow and its selected bounded leverage
> increments. The next work must define practical parameterized quantum routines
> with explicit qubit resources and non-recursive effects, then measurement-result
> storage and feedback. Target-side classical storage, branch execution, and wider
> control remain separate later contracts.

The appropriate public label remains **“experimental hybrid quantum–classical
language kernel with a tested local developer workflow.”** The terms
“production-ready,” “full interoperability,” “quantum hardware support,”
“general simulator,” and “fully operational language” should not be used for the
current repository.

## Evidence base and review method

This assessment treats a capability as verified only when a public contract,
focused fixture, and supported behavior exist together. That standard matches the
project architecture rule and prevents historical source trees from being
mistaken for active products.[7] The following levels keep local evidence and
remote evidence distinct.

| Evidence level | Meaning in this assessment | Current examples |
| --- | --- | --- |
| **Remotely validated** | The checked-in recovery profile passed in GitHub Actions. | 38 Linux CTests; 27 Windows and macOS platform-neutral CTests; three static-SDK consumer jobs; semantic inspection; local-only bounded mutable state; multi-register simulation; bounded identifier-if, negated identifier-if, and literal-negation-if export; and the Ubuntu-only language fixture set, including the Alpha Rust wrapper.[1] |
| **Locally validated** | The documented operation was executed in the review environment but is not itself a separate CI assertion. | The full 29-test recovery build and source-only Rust-wrapper Cargo conformance path before publication. |
| **Designed or planned** | The repository describes an intended subsystem but lacks the active contract and evidence required for a capability claim. | General runtime, target-side state/control execution, parameterized routines, measurement feedback, registry packages, provider access, and AI services.[2] [7] |

The distinction is material. The current six-job matrix establishes only the
scoped profile results named above. It does not establish cross-platform binary
compatibility, performance thresholds, package availability, a stable ABI, or a
general language implementation.[1] [3]

## What a developer can do today

The default profile builds `synq_lib` and the experimental
`synqc 0.1.0-experimental` command. The command validates the bounded source
profile, emits constrained OpenQASM 3 forms, renders a read-only semantic
environment, evaluates a declaration-only constant subset and a separate
bounded mutable-state subset, and calculates bounded local quantum
probabilities.[2] Its behavior is intentionally more
constrained than the long-term language vision.

| Workflow area | Verified current capability | Hard boundary |
| --- | --- | --- |
| Build and test | Build the default C++17 recovery profile and run 38 focused CTest checks on the Ubuntu full profile. Independent Windows/MSVC and macOS/Clang profiles each run 27 platform-neutral checks.[1] | Historical optional targets remain outside this profile, and the two non-Linux counts do not establish a support commitment.[3] |
| Diagnostics | Receive structured parser, semantic, resolver, evaluator, and simulator failures for supported constructs. | No lexer recovery, multi-error recovery, IDE/LSP protocol, or complete type diagnostics exists.[2] |
| Quantum source model | Use a small typed gate subset: `h`, `x`, `y`, `z`, `cx`, `bell_pair`, and literal-angle `rx`, `ry`, `rz`, or `p`, bounded typed measurement syntax, and Alpha named-register operands. | No general gate language, dynamic indexing, lifetime model, aliasing, deallocation, dynamic allocation, or hardware semantics exists.[2] [5] |
| Classical source model | Use literal declarations, earlier aliases, exact Alpha-gated one-operator Integer arithmetic and Boolean forms, plus feature-gated top-level `var`/`set` cells with fixed static types. `--inspect-semantics` reports binding/cell metadata without evaluation.[4] | No general expressions, nested scopes, decimal evaluation, callable calls, runtime values, target-side state, or executable classical control flow exists. |
| OpenQASM output | Export a bounded AST subset; strict Hybrid export supports declared named registers, one literal-`if` gate body, compile-time `not true/false`, one earlier Boolean-literal-declaration identifier-`if` gate body, or `not` over that identifier. The three control fixtures have reference-parser validation. | Export is source generation only; it does not execute circuits, prove semantic equivalence, fold nested negation, lower binary Boolean expressions/aliases/measurement results, or support named results. |
| Local quantum calculation | Calculate deterministic pure-state basis and marginal probability data across explicit default or named registers with declaration-order physical allocation and fixed combined limits.[5] | No sampling/collapse, noise, control-flow execution, provider connection, hardware submission, resource lifecycle, or dynamic allocation occurs. |
| Classical evaluation | Evaluate opt-in immutable Integer/Boolean/String literals, aliases, checked bounded expressions, and separate top-level mutable Boolean/Integer/String cells in source order. | This is not a general interpreter, target runtime, or branch executor. |
| Experimental controls | Require exact source opt-ins for the active Alpha language features. | The registry is not a package, permission, capability-security, or feature-lifecycle management system. |
| Native interoperability | The Ubuntu full profile exercises C, direct Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, test-only Mercury, and the source-only Alpha Rust wrapper over the opaque C ABI.[1] [6] | The non-C fixtures are Ubuntu-only execution evidence. There is no released wrapper, registry package, stable ABI, installed shared library, or general cross-platform SDK guarantee. |
| Delivery baseline | `synqc`, documentation, tested-environment guidance, contribution/security routes, and an experimental static CMake SDK path exist. Ubuntu 22.04, Windows MSVC, and macOS Clang independently prove static producer, install, discovery, compile, link, and C-consumer execution.[1] [3] | No signing, provenance attestation, reproducible-build proof, security audit, release matrix, or support SLA exists. |

## Safety, reliability, and usability assessment

The core safety approach remains appropriate for early language work. New
behavior is bounded by exact feature gates, support matrices, explicit opt-ins,
resource limits, structured errors, and negative fixtures. The evaluator and
simulator are opt-in operations, and the simulator rejects resource and semantic
shapes outside its documented model.[2] [5] This reduces the risk that the
presence of syntax is mistaken for a complete runtime.

The developer journey is coherent for the selected path: the CLI can validate,
inspect, export, evaluate, or simulate; diagnostics use defined classes; build
and test commands are documented; and the default profile does not silently
build known-incomplete subsystems.[2] [3] It is not yet “easy to learn” in the
broad language-design sense because there is no stable language specification,
editioned tutorial progression, general type system, REPL, package manager,
editor integration, or user-tested ergonomic study.

| Operational gate | Finding | Evidence status | Remaining condition before a stronger claim |
| --- | --- | --- | --- |
| G1 — Supported CLI | Complete for the bounded recovery workflow. | Remotely validated. | Define output-compatibility and deprecation policy before stability claims. |
| G2 — Executable semantic slice | Complete for bounded immutable evaluation, bounded local mutable state, and bounded local probability calculation. | Remotely validated. | Design general execution separately; do not extrapolate from the bounded evaluators/simulator. |
| G3 — Inspectable semantic slice | Complete for read-only top-level binding and mutable-cell metadata. | Remotely validated. | Define general runtime values, scopes, target-side storage, and branch effects separately before claiming execution. |
| G4 — Quantum developer loop | Complete for local validate/export/simulate exploration, including bounded named-register allocation. | Remotely validated. | Add provider or hardware paths only with separate credentials, safety, and integration evidence. |
| G5 — Tooling and diagnostics | Complete as a bounded compiler baseline. | Remotely validated. | Add stable textual/structured output policy, broader examples, and editor-facing diagnostics. |
| G6 — Interoperability contract | Complete only as an experimental opaque-C-ABI foundation, three-environment static-SDK proof, and source-only Alpha Rust-wrapper proof. | Remotely validated, narrowly scoped. | Add artifact lifecycle evidence and any package delivery one ecosystem at a time. |
| G7 — Performance and reliability | Build/test configuration is validated; no CI performance contract exists. | Remote build/test only. | Add reproducible benchmark matrix, regression thresholds, sanitizers, fuzzing, and security review. |
| G8 — Distribution and maintenance | Versioned artifact configuration, environment guidance, contribution process, security route, and issue templates exist. | Mixed: remote compiler baseline; local package/archive checks. | Add signing/provenance, maintained known-issues practice, and only then any support or distribution claim. |

## Material gaps

The remaining work is not cosmetic. Each gap is a necessary semantic or
public-contract decision. Advancing out of order risks exposing unstable parser
internals through bindings or packages before ownership and behavior are known.

| Priority | Remaining gap | Why it blocks a stronger operational claim | Required completion evidence |
| --- | --- | --- | --- |
| **P0** | Coherent typed semantic core | Immutable bindings and bounded local mutable cells exist, but general values, scopes, effects, measurement-result values, ownership, and target/runtime semantics remain undefined. | Written semantic subset, typed IR invariants, positive/negative tests, and no silent lowering gaps. |
| **P0** | Versioned ABI and delivery lifecycle | The opaque C ABI policy, three static-SDK CI environments, and source-only Rust proof are useful experimental records, not stable distribution. | ABI conformance matrix, artifact-version policy, signing/provenance plan, and a supported-platform matrix without premature stable-ABI claims. |
| **P1** | Wider target-side classical control model | One earlier Boolean-literal declaration, including one unary `not` over it, can now map to immutable target Boolean storage for one `if` gate. `not true/false` folds at compile time without target storage. Nested negation, binary expressions, aliases, writes, lifetime, measurement-result mapping, loops, and measurement-body rules remain undefined. | Explicit widening rules, exact output, parser/import validation, negative fixtures, and no execution claim unless separately designed. |
| **P1** | Callable bodies and calls | Declarations record names only. Bodies, parameter/return types, scopes, recursion, resource effects, and call behavior remain semantic decisions. | A non-recursive first subset, call/resource rules, diagnostics, and bounded lowering/execution fixtures. |
| **P1** | Resource lifecycle and hardware mapping | Declaration-order allocation is defined only for bounded local simulation. | Lifetime, aliasing, deallocation, dynamic-allocation, and hardware-mapping rules with dedicated tests, if the language later needs them. |
| **P2** | Language-specific delivery | The Alpha Rust wrapper has RAII ownership and Cargo conformance tests, but it is source-only and uses a build/test C ABI shared library. | A versioned and maintainable delivery method, compatibility policy, native-language ownership/error tests, and artifact evidence before any registry or stable-wrapper claim. |
| **P2** | Frontend-to-compiler integration | The Evidence Ledger is a factual static site, not a deployed compilation service. | Explicit service/data model, abuse limits, privacy policy, endpoint contract, end-to-end fixtures, and deployment evidence. |
| **P3** | Provider, hardware, AI, and optimization claims | These are not active verified capabilities. | Each needs an independently scoped architecture, threat model, provider contract, opt-in/consent mechanism, tests, and factual support matrix. |

## Recommended next implementation sequence

The next phase should remain inside the compiler repository and preserve the
successful pattern: one small contract, explicit non-goals, positive and negative
fixtures, documentation, local validation, remote Compiler Core evidence, then
publication. It should **not** attempt universal language integration, a general
runtime, or provider features at once.

### Stage A — Public contract, semantic inspection, and delivery leverage — completed bounded baseline

The experimental ABI/distribution policy documents ownership, change,
compatibility, unsupported surfaces, and artifact/install boundaries. Ubuntu
22.04 and Windows MSVC independently prove the static library/header/CMake
package external-C-consumer path. The Alpha `synq-alpha` Rust wrapper is a
dependency-free source-only Cargo package: it owns the opaque program handle via
RAII and exposes parse, OpenQASM 3 export, and ABI identity helpers. Its three
Cargo tests run in the Ubuntu full profile against the test/build C ABI shared
library.[1] [3] [6]

The same baseline includes read-only semantic-environment inspection, bounded
multi-register simulation, identifier-`if` target lowering from immutable
Boolean-literal declarations, unary negation over that same stored value, and
compile-time literal negation. These are completed leverage increments, not
evidence for runtime values, general resource management, or a distributed SDK.

### Stage B — Wider target-side control contract — next

The literal-`if`, compile-time literal-negation-`if`, one Boolean-literal-
declaration identifier-`if`, and one negated-identifier `if` gate
source-lowering slices are complete. Before accepting nested negation, aliases,
binary Boolean-expression conditions, loop, branch block, or measurement body,
SynQ must define
the target-side classical store: its representation, allowed writes, lifetime,
initial values, Boolean/Integer condition mapping, measurement-result behavior,
and interaction with quantum resource boundaries. The first widening increment
should choose target lowering **or** local execution, not imply both.

| Choice | Suitable only when | Minimum evidence |
| --- | --- | --- |
| **Target lowering** | The source construct has an exact OpenQASM 3 representation under a defined source subset. | Exact output, parser/import validation, rejection fixtures, and no claim of local execution. |
| **Local execution** | Classical/result semantics and bounded quantum interaction are fully specified. | State model, termination/resource limits, deterministic result behavior, error model, and simulator fixtures. |

### Stage C — Callable bodies and calls — after the control contract

Only after scopes, resource effects, and the chosen control model have a stable
minimal design should SynQ admit callable bodies. A safe first design should
exclude recursion, dynamic dispatch, unbounded allocation, implicit capture, and
uncontrolled effects. It must specify calling convention, visibility, arguments,
return rules, quantum resource passing, and lowering or execution support per
backend.

### Stage D — macOS static-SDK conformance — independent platform evidence

The macOS Clang smoke profile is valuable compiler/CLI/C-ABI evidence, but it
does not build, install, discover, compile, link, and run an external static SDK
consumer from a clean prefix. Add that distinct job before any macOS SDK or
general cross-platform packaging claim. Keep the separate platform scopes visible
in all public records.[1] [3]

### Stage E — Language-package and wrapper evolution — after delivery contracts

The Rust wrapper proves the first maintainable source-level ownership boundary;
it intentionally does not create a registry, binary distribution, or stable Rust
API. Any next language-specific package must be selected one at a time and must
define its native-library source, supported platforms, ownership/error behavior,
compatibility policy, conformance suite, and zero-budget maintenance model before
publication. The existing Common Lisp, Clojure, and Mercury fixtures remain test
consumers until they have their own contracts.[2] [6]

## What “fully operational” should mean for SynQ

For this project, “fully operational” should be a sequence of evidence-backed
claims rather than a single finish line. The next credible maturity milestone is
an **alpha language-core release candidate**, not a broad production platform.
It should meet all of the following conditions.

1. It specifies one coherent hybrid language subset, including values, scopes,
   register/resource rules, diagnostics, and at least one end-to-end supported
   execution or lowering path.
2. It has versioned CLI and C ABI policies, bounded package/install surfaces, and
   documented ownership/error behavior.
3. Its free-CI build/test matrix contains deterministic positive and negative
   fixtures, with published limitations for every supported feature.
4. Its performance statements are limited to reproducible workloads with
   hardware, command, build, iteration, and comparison details.
5. Its community and security processes tell external contributors how to
   participate and report problems without representing a solo project as an
   enterprise support organization.

Once those conditions are met, it would be reasonable to describe SynQ as an
**operational alpha programming-language project** for its explicit subset. It
would still not justify claims of general-purpose completeness, quantum-hardware
availability, enterprise readiness, or universal language interoperability.

## Decision record

The recommended immediate scope is now **practical bounded parameterized quantum
routines with explicit qubit resources and alias rejection, followed by typed
measurement-result storage and feedback design**. This order preserves the
completed semantic-inspection, multi-register-simulation, source-only Rust-wrapper,
strict `if` lowering, and local-only mutable-state leverage work while avoiding
unsupported target-side state or branch-execution surface area. It provides prospective
contributors and adopters a clearer, reproducible contract than unverified
breadth and is more credible for community growth, revenue exploration, and
future acquisition discussions.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32242711770 "SynQ Compiler Core platform matrix — bounded Alpha mutable classical state"

[2]: ./PROJECT_STATUS.md "Current SynQ Project Status"

[3]: ./TESTED_ENVIRONMENTS.md "Tested Environments and Clean Installation"

[4]: ./ALPHA_SEMANTIC_KERNEL.md "Alpha Semantic Kernel"

[5]: ./MULTI_REGISTER_SIMULATION.md "Multi-Register Simulation"

[6]: ./EXPERIMENTAL_RUST_WRAPPER.md "Experimental Rust Wrapper"

[7]: ./ARCHITECTURE_ROADMAP.md "SynQ Architecture Roadmap"
