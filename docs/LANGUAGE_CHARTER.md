# SynQ Language Charter

**Status:** Design charter; not a language specification or a claim that the
listed future features are implemented.  
**Author:** Manus AI  
**Last reviewed:** 13 August 2026

## Purpose

SynQ is intended to become a **learnable hybrid quantum–classical programming
language**. Its purpose is not to replace every existing language, framework,
or quantum SDK. Its purpose is to offer one coherent language surface in which
classical logic, quantum kernels, and carefully bounded AI-assisted workflows
can be expressed without hiding the boundaries between them.

The project is being developed by a solo maintainer with no funded team or
hosted infrastructure budget. That constraint is an architectural input rather
than a disclaimer. SynQ must therefore prefer a small, well-tested kernel;
stable portable interfaces; ordinary build tools; and incremental evidence over
large promises. A capability is only described as supported after a repository
test verifies it and the documentation names the boundary of that support.

> **SynQ’s working promise:** make hybrid quantum–classical programs easier to
> read, validate, exchange, and evolve, while being explicit about what is
> classical, quantum, experimental, external, or not yet available.

## Current evidence and non-claims

The recovered compiler core currently parses a deliberately small,
line-oriented profile: declarations, `print`, `delay`, `ai`, and `quantum`
instructions. The quantum profile accepts selected explicit `q[index]`
operands and literal-angle parameter forms; the OpenQASM 3 exporter supports a
bounded gate set. The project’s current executable evidence is the source and
smoke-test suite, not a complete SynQ language implementation.[1] [2]

| Area | Verified today | Deliberately **not** claimed today |
| --- | --- | --- |
| Parsing | A recovery-profile parser with comments, declarations, selected instructions, explicit qubit operands, and limited literal-angle syntax. | A complete grammar, function system, scope model, type checker, macro system, or expression evaluator. |
| Quantum exchange | Bounded generation of OpenQASM 3 source for `h`, `x`, `y`, `z`, `cx`, `bell_pair`, `rx`, `ry`, `rz`, and `p`, with independent parser/import checks. | Quantum circuit execution, hardware submission, noise modelling, resource estimation, or semantic equivalence with any runtime. |
| Classical and AI surfaces | Source text can be represented as declaration, `print`, `delay`, or `ai` instructions in the recovery profile. | Classical execution, an AI runtime, model access, an autonomous agent, or an ML framework binding. |
| Interoperability | Source-level OpenQASM 3 export plus a locally tested, opaque-handle C ABI for parsing and exporting the same bounded profile. | A Rust, Mercury, Common Lisp, Clojure, JVM, Python, or universal language SDK. |

The project will not use a future roadmap to imply that a feature is usable
now. This distinction is especially important in quantum computing, where
valid emitted syntax, a semantically valid circuit, a simulator result, and a
hardware result are different kinds of evidence.

## The language model

SynQ’s intended model has three explicit layers. They share names, types, and
diagnostics, but they must not silently erase one another’s constraints.

| Layer | Intended responsibility | Rules that preserve clarity |
| --- | --- | --- |
| **Classical orchestration** | Values, typed functions, branching, loops, I/O boundaries, and composition of computation. | Classical effects are named and typed. A future compiler must not treat source text merely as executable host-language code. |
| **Quantum kernel** | Qubit allocation, gate application, measurement, circuit composition, target-independent analysis, and exchange backends. | Quantum resources, measurement boundaries, and backend requirements remain visible in the source and IR. Export is not execution. |
| **AI-assisted workflow** | Opt-in analysis, suggestions, synthesis proposals, and provenance-bearing transformations. | An AI suggestion is data until the developer accepts it. It cannot silently change a program or a quantum circuit. Every generated change must be inspectable and testable. |

The initial pedagogical principle is **progressive disclosure**. A beginner
should be able to read a small kernel such as `quantum h q[0]` and see the
corresponding OpenQASM form. Advanced capabilities must add explicit syntax and
diagnostics instead of making simple programs mysterious. The language will
prefer familiar constructs, small orthogonal vocabulary, examples that compile,
and precise error messages over clever syntax.

## Design principles

SynQ’s design decisions must meet the following principles. A proposed feature
that fails one of them should remain an experiment or be rejected.

| Principle | Meaning in SynQ | Practical consequence |
| --- | --- | --- |
| **Explicit boundaries** | Source must distinguish classical control, quantum operations, experimental behavior, and calls across language boundaries. | No implicit hardware submission, hidden transpilation, unannounced host-language execution, or ambient AI action. |
| **Safe evolution by opt-in** | Unstable ideas must be named, disabled by default, versioned, and removable. | Alpha features require a source or build opt-in and emit a diagnostic; removed feature names remain diagnosable rather than silently changing meaning. |
| **Portable first** | The first durable compatibility boundary should work with conventional native toolchains. | The C ABI is the first native interoperability target; language-specific wrappers come only after contract tests. |
| **Inspectable transformations** | Programs, IR, exported artifacts, diagnostics, and generated suggestions should be reviewable. | Every lowering and backend target needs deterministic tests and a documented supported subset. |
| **Correctness before breadth** | A small behavior with independent checks is more valuable than a broad unverified surface. | One gate mapping or one FFI call is added with negative tests before the next category is claimed. |
| **Zero-budget sustainability** | The language must remain buildable and reviewable with free tools and modest CI resources. | Avoid required proprietary services, paid hardware access, or a mandatory cloud control plane. |

## Interoperability: a precise commitment

“Full interoperability” is an aspiration that needs a precise, testable
meaning. SynQ will pursue **contract interoperability**, not an immediate claim
that it can import, execute, or preserve the complete semantics of every major
programming language.

The first general native contract will use a versioned **C ABI** with opaque
handles, fixed-width status values, UTF-8 strings, documented ownership, and no
C++ object types in public signatures. This is a pragmatic universal bridge:
Rust documents ABI selection and exported symbols; Mercury exposes a foreign
language interface including C on C-compiling backends; CFFI exists to call C
functions from portable Common Lisp; and Clojure interoperates with Java,
making a small JVM facade the suitable later bridge for Clojure.[3] [4] [5] [6]

| Ecosystem | First credible SynQ route | What SynQ will not claim until separately tested |
| --- | --- | --- |
| **C and C++** | `synq_ffi.h`, a C-callable library, opaque program handles, and a compiled C consumer smoke test. | Stable C++ ABI compatibility, because the public contract is deliberately C rather than C++. |
| **Rust** | A small safe wrapper over `extern "C"` functions, with ownership represented by Rust types. | A Rust-native ABI, automatic binding generation, or direct use of C++ compiler internals. |
| **Mercury** | A package that maps C ABI calls through Mercury’s `pragma foreign_proc` on an applicable C backend. | Support across every Mercury backend or complete bidirectional semantic translation. |
| **Common Lisp** | A CFFI package that loads the library and declares only the verified functions. | Support for every Common Lisp implementation, callbacks, or full automatic object marshalling. |
| **Clojure** | A thin Java/JVM facade that Clojure calls through ordinary Java interop; that facade may use JNI only after a narrow JNI contract is tested. | A claim that Clojure calls the C ABI directly, or that a native/JNI bridge is portable without platform testing. |
| **Other languages** | The same C ABI, stable text formats, and later generated bindings where an ecosystem has a maintained FFI route. | “Compatible with every language” until that ecosystem has documented, automated contract tests. |

This strategy is intentionally asymmetric. SynQ will first support **calling
SynQ services from another language**—for parsing, diagnostics, controlled
lowering, and artifact export. Calling arbitrary foreign functions *from*
SynQ, embedding foreign garbage-collected runtimes, and source-to-source
translation between programming languages are distinct projects. They are not
preconditions for a useful interoperable language kernel.

## Experimental feature policy

Experimental features are necessary for a language exploring a young technical
field, but “experimental” is not permission for silently unstable behavior.
SynQ will use a registry and named gates inspired by the public governance
patterns of Rust and Kubernetes: each feature has an explicit identifier,
stage, introduction information, tracking reference, default state, and
removal or graduation path. Rust’s compiler guidance records unstable features
with a version and tracking issue, while Kubernetes exposes staged features as
explicit `key=value` flags and records their state transitions.[7] [8]

### Stages and defaults

| Stage | Default | Source/build requirement | Compatibility promise |
| --- | --- | --- | --- |
| **Alpha** | Disabled | An exact opt-in is required. The compiler warns that the feature may change or disappear. | No source, ABI, IR, or behavior stability promise. Use only in isolated experiments. |
| **Beta** | Disabled until a documented decision enables it for a release profile | An explicit opt-in remains required unless release notes state otherwise. | Syntax and diagnostics are tested, but breaking changes remain possible with a migration note. |
| **Stable** | Enabled | No feature gate is required. | Subject to the published compatibility policy and deprecation process. |
| **Removed** | Rejected | An old name produces an actionable error with a migration or removal reason. | No silent re-interpretation. |

The first source-facing shape is deliberately readable and is implemented by
the recovery parser as a **file-scoped** opt-in:

```synq
#[experimental(feature = "parameterized-quantum-gates")]

quantum rx(pi/2) q[0]
```

Feature metadata belongs in the compiler registry rather than being trusted
from arbitrary source text. The current registry records the feature name,
stage, concise description, and tracking reference; the parser rejects an
unknown annotation and rejects parameterized gates without the explicit opt-in.
The parser API can also enable a registered feature for one parser instance,
which is exercised by a smoke test. Source editions, feature introduction
versions, removal records, and structured warnings are planned extensions—not
current claims.

No gate makes an inherently unsafe quantum, AI, or native operation safe. The
gate only makes a dependency on unstable language behavior visible and
reviewable. Hardware credentials, remote execution, model-provider credentials,
unsafe native memory, and network policy require their own explicit designs and
will not be implied by this feature system.

## Compatibility and evolution rules

SynQ will version its interfaces separately enough to prevent accidental
coupling: source edition, IR schema, C ABI, and backend capability set can move
at different rates. A backend must reject unknown or unsupported operations
with a source location where possible; it must not emit a different operation
as a convenience. A language binding must pin the C ABI major version it
expects. A compiler release must document gates that have been added,
graduated, deprecated, or removed.

The language will not promise perfect cross-backend equivalence. Instead,
supported behavior will be described as a capability matrix with exact test
fixtures. The existing OpenQASM exporter is a model for this practice: it names
the accepted source forms, the supported mappings, rejection cases, and two
independent downstream validations.[2]

## Milestone-based direction

The roadmap is deliberately expressed as graduation conditions, not calendar
dates. A zero-budget project should not give false precision about delivery
times. The architecture roadmap contains the implementation detail and test
gates for each milestone.

| Milestone | Outcome required before it is called complete |
| --- | --- |
| **0. Recovery baseline** | Maintained compiler-core smoke profile, factual project-status record, and bounded OpenQASM 3 validation. This is the current position. |
| **1. Language foundation** | The first registry/enforcement and C ABI/C-consumer increments are locally tested. The milestone is not complete until the full project checks, evidence record, commit, and CI publication are complete. |
| **2. Hybrid core** | Explicit classical expressions, control flow, typed quantum resources, measurement/result boundaries, and a testable hybrid IR. |
| **3. Native ecosystem bridge** | Versioned C ABI release discipline and a tested Rust wrapper. Mercury and Common Lisp bindings are added only with their own toolchain tests. |
| **4. JVM and language SDKs** | A narrow JVM facade and Clojure example with automated tests; additional language bindings are maintained only where CI can verify them. |
| **5. Advanced research track** | Optional AI-assisted analysis, additional backends, optimizer research, and experimental features that meet the documented alpha/beta graduation criteria. |

## Non-goals for the present project phase

SynQ is not currently a hardware control plane, a hosted quantum platform, a
replacement for Qiskit or every other SDK, a general AI agent platform, or a
universal source-language translator. It does not currently promise production
deployment, fault tolerance, quantum advantage, cryptographic security, or
revenue outcomes. The dual licensing model may support future commercial work,
but licensing does not prove technical maturity.

The project will refuse scope that prevents a solo maintainer from validating
behavior. In practice, that means preferring a narrow portable contract over
several untested bindings, and preferring one complete documented example over
many aspirational syntax sketches.

## How this charter is maintained

This charter is a living design boundary. Any pull request that changes the
language surface, experimental status, public C ABI, or backend claims must
update the relevant capability table and include a test. Every public example
must be labelled either **verified** with a command and expected result, or
**illustrative** and not represented as executable SynQ today.

## References

[1]: https://github.com/TangoSplicer/SynQ/blob/ceaa971/compiler/src/compiler/parser.cpp "SynQ recovery parser source at recovery baseline"
[2]: ./INTEROPERABILITY.md "SynQ OpenQASM interoperability boundary and validators"
[3]: https://doc.rust-lang.org/reference/abi.html "The Rust Reference: Application binary interface"
[4]: https://mercurylang.org/information/doc-release/mercury_user_guide/Foreign-language-interface.html "The Mercury User’s Guide: Foreign language interface"
[5]: https://cffi.common-lisp.dev/manual/cffi-manual.html "CFFI User Manual"
[6]: https://clojure.org/reference/java_interop "Clojure Java Interop"
[7]: https://rustc-dev-guide.rust-lang.org/feature-gates.html "Rust Compiler Development Guide: Feature Gates"
[8]: https://kubernetes.io/docs/reference/command-line-tools-reference/feature-gates/ "Kubernetes Feature Gates"
