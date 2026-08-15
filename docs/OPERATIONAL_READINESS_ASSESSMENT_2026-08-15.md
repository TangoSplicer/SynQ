# SynQ Operational-Readiness Assessment — 15 August 2026

**Assessment status:** Evidence-based technical assessment, not a release announcement.  
**Assessment scope:** The default CMake recovery profile, its documented `synqc`
workflow, the public experimental C ABI, and the factual public-facing project
position.  
**Evidence cut-off:** Commit `df571ce`, which records successful delivery-baseline
evidence from Compiler Core #45 for implementation commit `202ebaf`.[1] [2]

## Executive assessment

SynQ is **not a fully operational programming-language platform**. It is an
**operational experimental language seed**: a narrowly specified and tested
compiler workflow that a developer can build, validate, diagnose, export from,
evaluate in a bounded classical subset, and simulate in a bounded quantum subset.
The default recovery profile has passed **26/26** focused checks both locally and
in Compiler Core #45.[1] The observed workflow is usable for exploration of its
documented subset, but it has not earned claims of stable compatibility, complete
language semantics, deployment readiness, quantum-hardware operation, or
production support.

> **Readiness finding.** SynQ has completed its operational-baseline gates for a
> bounded experimental developer workflow. The next work must convert that
> baseline into a coherent language-core contract before adding breadth such as
> providers, AI services, or language-specific SDKs.

The appropriate public label is therefore **“experimental hybrid
quantum–classical language kernel with a tested local developer workflow.”** The
terms “production-ready,” “full interoperability,” “quantum hardware support,”
“general simulator,” and “fully operational language” should not be used for the
current repository.

## Evidence base and review method

This assessment treats a capability as verified only when a public contract,
focused fixture, and supported behavior exist together. That standard matches the
project’s architecture rule and prevents historical source trees from being
mistaken for active products.[3] It distinguishes three evidence levels:

| Evidence level | Meaning in this assessment | Examples |
| --- | --- | --- |
| **Remotely validated** | The checked-in recovery profile passed in the GitHub Actions Compiler Core workflow. | Build, 26 focused CTest checks, versioned CLI smoke path, C/Rust/Common Lisp/Clojure/Mercury opaque-ABI consumers.[1] |
| **Locally validated** | The documented operation was executed in the review environment but is not itself a CI assertion. | CMake installation, CPack ZIP creation, and the fixed local benchmark timing.[4] [5] |
| **Designed or planned** | The repository describes an intended subsystem but does not provide the required active contract and evidence. | General runtime, provider support, QIR/LLVM targets, safe language packages, AI transformation services.[3] |

The distinction is material. Compiler Core #45 confirms that the delivery-baseline
source compiles and its 26 focused checks pass; it does **not** execute the local
installation, CPack archive, or timing command, and it does not establish
cross-platform binary compatibility or a performance threshold.[1] [4] [5]

## What a developer can do today

The current default profile builds `synq_lib` and the experimental
`synqc 0.1.0-experimental` command. The command validates a bounded source
profile, emits two constrained OpenQASM 3 forms, evaluates a bounded
declaration-only constant subset, and calculates bounded local quantum
probabilities.[2] [3] Its behavior is intentionally more constrained than the
long-term language vision.

| Workflow area | Verified current capability | Hard boundary |
| --- | --- | --- |
| Build and test | Build the default C++17 recovery profile and run 26 focused CTest checks. | Historical optional targets remain excluded because they are not verified by this profile.[2] |
| Diagnostics | Receive structured parser, semantic, resolver, evaluator, and simulator failures for supported constructs. | No lexer recovery, multi-error recovery, IDE/LSP protocol, or complete type diagnostics exists.[3] |
| Quantum source model | Use a small typed gate subset: `h`, `x`, `y`, `z`, `cx`, `bell_pair`, and literal-angle `rx`, `ry`, `rz`, or `p`, with bounded typed measurement syntax. | No general gate language, dynamic indexing, named-register use in the supported execution/export path, ownership model, or hardware semantics exists.[3] |
| Classical source model | Use literal declarations, earlier aliases, and exact Alpha-gated one-operator Integer arithmetic; exact Alpha-gated Boolean control shapes are represented. | No general expressions, assignment, scopes, decimal evaluation, callable calls, or executable classical control flow exists.[3] |
| OpenQASM output | Export a bounded AST subset; strict Hybrid export supports one explicit default `qubit q[n]` declaration and rejects unsupported semantics. Output is externally parser/import checked. | Export is source generation only; it does not execute circuits, prove semantic equivalence, lower controls, or support named registers/results.[3] |
| Local quantum calculation | Calculate deterministic pure-state basis and marginal probability data for one default register, supported gates, and optional unnamed trailing measurements. Default limits are 10 qubits and 1,024 gates. | No sampling/collapse, noise, control-flow execution, provider connection, or hardware submission occurs.[3] |
| Classical evaluation | Evaluate opt-in Integer/Boolean/String literals, earlier aliases, and checked single-operator Integer arithmetic. | This is not a general interpreter or mutable runtime.[3] |
| Experimental controls | Require exact source opt-ins for the active Alpha language features. | The registry is not a package, permission, capability-security, or feature-lifecycle management system.[3] |
| Native interoperability | Exercise an opaque v1 C ABI from C, Rust, Common Lisp CFFI, Clojure/JNA, and Mercury C-backend test consumers. | No stable ABI promise, installed shared library, Rust crate, safe wrapper, Clojure library, Lisp package, or Mercury package exists.[2] [3] |
| Delivery baseline | Install `synqc` and three workflow documents locally; generate an experimental CPack ZIP locally; build optional warning/ELF-hardening configuration and an opt-in benchmark. | No signing, provenance attestation, reproducible-build proof, security audit, cross-platform release matrix, or support policy exists.[4] [5] |

## Safety, reliability, and usability assessment

The core safety approach is appropriate for early language work. New behavior is
bounded by exact feature gates, support matrices, explicit opt-ins, resource
limits, structured errors, and negative fixtures. In particular, the evaluator
and simulator must be explicitly enabled by their caller, and the simulator
rejects resource and semantic shapes outside its documented model.[3] This
reduces the risk that syntax presence is mistaken for a complete runtime.

The developer experience is now coherent enough for the selected path: the CLI
can validate, export, evaluate, or simulate; errors use diagnostic classes; the
build, test, and delivery commands are written down; and the default profile
does not silently build known-incomplete subsystems.[2] [4] The experience is
not yet “easy to learn” in the broad language-design sense because there is no
language specification, tutorial progression verified against a stable edition,
general type system, REPL, package manager, editor integration, or user-tested
ergonomic study.

| Operational gate | Finding | Evidence status | Remaining condition before a stronger claim |
| --- | --- | --- | --- |
| G1 — Supported CLI | Complete for the bounded recovery workflow. | Remotely validated. | Define output-compatibility and deprecation policy before stability claims.[2] |
| G2 — Executable semantic slice | Complete for bounded constant evaluation and local probability calculation. | Remotely validated. | Design general execution separately; do not extrapolate from the bounded evaluator/simulator.[3] |
| G3 — Quantum developer loop | Complete for local validate/export/simulate exploration. | Remotely validated. | Add provider or hardware paths only with distinct credentials, safety, and integration evidence.[3] |
| G4 — Tooling and diagnostics | Complete as a bounded compiler baseline. | Remotely validated. | Add stable textual/structured output contract, broader examples, and editor-facing diagnostics.[2] |
| G5 — Interoperability contract | Complete only as an experimental opaque-C-ABI foundation. | Remotely validated. | Publish versioning/ownership/compatibility policy, packaged artifacts, and safe language adapters.[2] [3] |
| G6 — Performance and reliability | Build/test configuration is validated; the recorded throughput is not a CI performance result. | Mixed: remote build/test; local timing. | Add reproducible benchmark matrix, regression thresholds, sanitizers, fuzzing, and security review.[1] [5] |
| G7 — Distribution and maintenance | Versioned artifact configuration and instructions exist. | Mixed: remote compiler baseline; local install/archive. | Add releases/tags, signing/provenance policy, disclosure route, support policy, and known-issues process.[1] [4] |

## Material gaps

The remaining work is not cosmetic. Each gap below represents either a necessary
language semantic decision or a public-contract decision. Advancing out of order
would increase the risk of locking fragile parser internals into multiple
bindings or package formats.

| Priority | Gap | Why it blocks a stronger operational claim | Required completion evidence |
| --- | --- | --- | --- |
| **P0** | Versioned ABI and distribution policy | The experimental compatibility, ownership, and distribution boundary is published in [`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md), and a static native SDK clean-install path passed remotely in [Compiler Core #46](https://github.com/TangoSplicer/SynQ/actions/runs/31898767207), recorded in [`EXPERIMENTAL_NATIVE_SDK.md`](./EXPERIMENTAL_NATIVE_SDK.md). The C ABI remains neither stable nor distributable as a supported SDK. | ABI conformance matrix, artifact-version policy, signing/provenance, and a supported-platform matrix without claiming stable ABI prematurely. |
| **P0** | Coherent typed semantic core | The current AST and HIR are intentionally narrow; general values, scopes, effects, result values, and ownership are not defined. | A written semantic subset, typed IR invariants, positive/negative tests, and no silent lowering gaps. |
| **P1** | Named-register operands and resource semantics | Current supported simulation/export relies on one default `q[n]` register. General named registers need unambiguous allocation, resolution, and lowering rules. | Syntax and HIR design record; range/forward/duplicate/error fixtures; exporter/simulator support matrix; independent OpenQASM validation where supported. |
| **P1** | Control-flow lowering or execution model | Alpha control syntax is represented but not generally executed or lowered. | Decide whether each bounded `if`/`while` shape has an exact target mapping; implement only a tested subset and reject the rest explicitly. |
| **P1** | Callable bodies and calls | Declarations record names only. Bodies, parameter/return types, scopes, recursion, and resource effects are all semantic decisions. | A deliberately non-recursive first subset, call graph/resource rules, escape behavior, diagnostics, and bounded execution/lowering fixtures. |
| **P2** | Language-specific adapters | Existing foreign-language checks prove C ABI reachability, not idiomatic or safe interoperability. | Versioned C ABI policy first; then one packaged wrapper at a time with native-language ownership/error tests. |
| **P2** | Frontend-to-compiler integration | The frontend builds and tests locally but does not establish a deployed compilation service. | Explicit service/data model, abuse limits, privacy policy, endpoint contract, end-to-end fixtures, and deployment evidence. |
| **P2** | Community governance and maintenance materials | A solo-maintained experimental language needs contribution, issue triage, and security-reporting expectations before broad adoption. | Contribution guide, issue/PR templates, code of conduct or scope policy, security contact/disclosure process, and a maintained known-issues ledger. |
| **P3** | Provider, hardware, AI, and optimization claims | These are not active verified capabilities. | Each requires an independently scoped architecture, threat model, provider contract, opt-in/consent mechanism, tests, and factual support matrix. |

## Recommended next implementation sequence

The next phase should stay inside the compiler repository and preserve the
project’s successful pattern: one small contract, explicit non-goals, positive
and negative fixtures, a documentation record, local validation, remote
Compiler Core evidence, then publication. It should **not** attempt to make all
language integrations or provider features work at once.

### Stage A — Public-contract foundation

First, write and adopt an **experimental ABI/distribution policy**. The policy
should state what ABI v1 covers, its ownership rules, compatibility target,
unsupported surfaces, deprecation/major-version process, and artifact/install
status. It must not call the ABI stable. This stage provides the boundary needed
before a Rust crate, CFFI package, Clojure library, or Mercury package can be
honestly advertised.

In parallel only where it does not alter ABI behavior, create the maintenance
materials: `CONTRIBUTING.md`, a security-reporting route using a free dedicated
personal email, issue templates, a pull-request template, and a short
known-issues record. These documents lower contributor ambiguity without
pretending that a support organization or paid domain exists.

### Stage B — Named-register semantic design and bounded implementation

Next, design named-register operands before writing their parser support. The
design must answer which declarations allocate a resource, whether default and
named registers may coexist, how operands resolve, whether indices must be
literal, and how every target handles unsupported cases. The first implementation
should be bounded—such as positive-size typed declarations plus literal indexed
operands—then add parser, resolver, Hybrid IR, exporter, simulator, CLI, and
negative tests only for semantics that are exact.

For OpenQASM, a support matrix must say whether each named-register source
construct maps directly or is rejected. The exporter must never rewrite a
register name or collapse multiple registers without a documented, tested
semantic preservation rule.

### Stage C — Bounded control-flow target decision

The project should choose one of two explicit paths for each typed control form:

| Option | When it is appropriate | Minimum evidence |
| --- | --- | --- |
| **Target lowering** | A construct has an exact OpenQASM 3 representation under a defined source subset. | Exact emitted output, parser/import validation, rejection fixtures, and no claim of execution. |
| **Local execution** | The construct has a fully specified classical/result semantics and bounded quantum interaction. | State model, termination/resource limits, deterministic result behavior, error model, and simulator fixtures. |

The first bounded `if` lowering is a more tractable next step than `while`
execution. A loop should remain rejected until its termination, quantum-state,
measurement-result, and resource semantics are written down. This protects
“experimental” from becoming a label for unspecified behavior.

### Stage D — Callable bodies and calls

Only after scopes, registers, and control semantics have a stable minimal design
should SynQ admit callable bodies. A safe first design would exclude recursion,
dynamic dispatch, unbounded allocation, implicit capture, and uncontrolled
effects. It should specify calling convention, visibility, arguments, return
rules, quantum resource passing, and lowering/execution support per backend.

### Stage E — One interoperable distribution proof

After Stage A and one semantic increment, select **one** host language for an
idiomatic alpha adapter. Rust is the logical first candidate because the project
already has a remotely validated Rust opaque-C-ABI consumer, but that is a
technical sequencing choice—not a claim that Rust support is complete.[2] The
wrapper should be versioned, avoid exposing C++ internals, have ownership/error
tests, and be distributed only through a method the project can maintain at zero
budget. The other language consumers should remain test consumers until their
own package and lifecycle work is completed.

## What “fully operational” should mean for SynQ

For this project, “fully operational” should be a series of evidence-backed
claims rather than a single finish line. The next credible maturity milestone is
an **alpha language-core release candidate**, not a broad production platform.
It should meet all of the following criteria:

1. It specifies one coherent hybrid language subset, including values, scopes,
   register/resource rules, diagnostics, and at least one end-to-end supported
   execution or lowering path.
2. It has versioned CLI and C ABI policies, package/install boundaries, and
   documented ownership/error behavior.
3. Its build/test matrix runs in free CI, with deterministic fixtures, negative
   cases, and published limitations for every supported feature.
4. Its performance claims are limited to reproducible workloads with hardware,
   command, build, iteration, and comparison details.
5. Its community and security processes tell external contributors how to
   participate and how to report problems without representing a solo project as
   an enterprise support organization.

Once those conditions are met, it would be reasonable to describe SynQ as an
**operational alpha programming-language project** for its explicit subset. It
would still not justify claims of general-purpose completeness, quantum hardware
availability, enterprise readiness, or universal language interoperability.

## Decision record

The recommended immediate scope is **Stage A followed by Stage B**: establish
the public-contract and maintenance boundary, then add named-register semantics
through a narrowly designed, test-first slice. This directly improves the
language rather than creating unsupported surface area. It also supports the
long-term goals of community growth, revenue, and acquisition interest more
credibly than unverified breadth because prospective contributors and adopters
can inspect a clear, reproducible contract and a transparent capability boundary.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31887461976 "SynQ Compiler Core #45 — Add experimental delivery baseline"

[2]: https://github.com/TangoSplicer/SynQ/blob/df571ce/docs/PROJECT_STATUS.md "SynQ Project Status at assessment evidence cut-off"

[3]: https://github.com/TangoSplicer/SynQ/blob/df571ce/docs/ARCHITECTURE_ROADMAP.md "SynQ Architecture Roadmap"

[4]: https://github.com/TangoSplicer/SynQ/blob/df571ce/docs/PACKAGING.md "Recovery-Profile CLI Packaging"

[5]: https://github.com/TangoSplicer/SynQ/blob/df571ce/docs/PERFORMANCE.md "Recovery-Profile Performance Evidence"
