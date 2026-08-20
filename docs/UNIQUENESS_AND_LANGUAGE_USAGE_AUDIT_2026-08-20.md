# SynQ Differentiation and Language-Usage Audit

**Status:** Evidence-led assessment of the experimental recovery profile.  
**Reviewed:** 20 August 2026.  
**Evidence baseline:** [Compiler Core #32369872013](https://github.com/TangoSplicer/SynQ/actions/runs/32369872013), reliability revision `2965ec4`.

## Decision

SynQ must **not** claim to be globally unique, the first hybrid quantum language,
or a replacement for Qiskit, Cirq, PennyLane, Q#, OpenQASM, or Silq. Such claims
would require an exhaustive and continually renewed survey that this repository
does not possess. Instead, the defensible position is narrower: the current
recovery profile combines a **feature-gated, bounded SynQ source subset**, an
**inspectable compiler-to-target path**, a **bounded local evaluator**, explicit
failure boundaries, and **tested foreign-consumer ABI proofs**. The combination
is a deliberate project direction, not proof of worldwide exclusivity.

> SynQ’s differentiator is **auditable boundedness**: a source form is promoted
> only when its parser, semantic boundary, execution or lowering behavior,
> fixtures, and remote evidence can be named. Unsupported behavior must reject
> rather than silently imitate a broader runtime.

## Method and scope

This audit compared current repository evidence with official descriptions of
Qiskit, Q#, Cirq, PennyLane, OpenQASM 3, and Silq. It also examined the primary
SynQ entry points, the supported CMake recovery build, the checked-in foreign
consumer fixtures, and tracked non-SynQ material. It is a product and
documentation audit, not a legal novelty opinion, patent search, security
certification, performance benchmark, or exhaustive study of all quantum
software.

| Audit question | Result | Evidence boundary |
| --- | --- | --- |
| Are current beginner entry points SynQ-first? | **Yes.** The root README and the supported beginner tutorials show SynQ source for SynQ programs. | This does not prove every historical `.synq` artifact is supported by the recovery compiler. |
| Is the verified compiler itself written in SynQ? | **No.** The recovery compiler is a C++17 implementation. | SynQ is not self-hosting or bootstrapped; claiming otherwise would be false. |
| Is Python a runtime or teaching prerequisite for SynQ programs? | **No.** It is not used to express the supported SynQ examples or to run SynQ semantics. | The CMake profile uses Python only for external OpenQASM parser/importer validation. |
| Are foreign-language files removed entirely? | **No, by design.** C, Rust, Common Lisp, Clojure, and Mercury fixtures prove a narrow opaque C ABI consumer boundary. | These are not language packages, source translators, or evidence that SynQ programs should be written in those languages. |
| Is SynQ globally unique? | **Not established and not claimed.** | Individual properties overlap with existing languages and SDKs; the project documents a particular evidence-gated combination. |

## External comparison

| Ecosystem | Officially documented scope | What SynQ must not claim | Defensible SynQ distinction today |
| --- | --- | --- | --- |
| Qiskit | IBM describes a stack for building, optimizing, and executing workloads, with circuit/operator/primitives tooling, heterogeneous orchestration, and hardware plugins.[1] | Comparable execution breadth, provider support, transpilation performance, orchestration, or ecosystem scale. | A smaller C++ compiler kernel that exposes its accepted/rejected subset and evidence trail. |
| Q# | Microsoft describes a high-level language and QDK with qubit management, quantum/classical integration, hardware-agnostic mapping, simulators, and hardware submission.[2] | A complete language/runtime, general qubit lifecycle model, hardware portability, or provider execution. | Explicit experimental feature declarations and narrowly documented local evaluation/lowering boundaries. |
| Cirq | Google documents a Python circuit library with build, simulation, transformation, noise, experiment, and hardware workflow areas.[3] | A replacement circuit toolkit, noise model, or broad device workflow. | SynQ source is its own bounded language surface, while its foreign C ABI consumers are independently exercised. |
| PennyLane | PennyLane documents a Python library for automatic differentiation, hybrid models, JIT compilation, plugins, and optimization/ML tools.[4] | Gradients, differentiable programming, hybrid JIT execution, unbounded loops, or broad device support. | A purposefully non-general local evaluator with strict resource bounds and explicit rejection paths. |
| OpenQASM 3 | OpenQASM 3 is an imperative language and target-oriented intermediate representation with feedback, classical flow, timing, and pulse-level mechanisms; implementations may reject unsupported runtime operations.[5] | Full OpenQASM support, an OpenQASM runtime, target execution, or semantic equivalence beyond validated exports. | SynQ exports a strict, separately tested Hybrid OpenQASM subset and rejects nodes without a mapping. |
| Silq | ETH Zürich describes Silq as a high-level quantum language with a strong static type system and safe automatic uncomputation.[6] | Novelty in high-level quantum languages, static safety, automatic uncomputation, or physicality checks. | Safety evidence based on explicit feature gates, bounded semantics, diagnostics, and named test fixtures—not a claim of Silq-equivalent semantics. |

## The current combined SynQ position

The repository can substantiate the following combined properties, provided that
each remains qualified by the current experimental boundary.

| Combined property | Current repository evidence | Important exclusion |
| --- | --- | --- |
| **Feature-gated language growth** | Parser and semantic paths register Alpha/Beta/Stable experimental features, and the documented U1–U6 slices require explicit opt-in declarations. | A feature gate is not a stability guarantee, compatibility promise, or general safety proof. |
| **Inspectable hybrid boundary** | `synqc` validates, emits strict Hybrid OpenQASM where defined, inspects semantic metadata, evaluates bounded local values, and locally simulates a limited probability model. | Export and simulation are not hardware execution, provider integration, or a general runtime. |
| **Bounded local classical execution** | U5/U6 evaluate particular non-capturing one- and two-formal callable shapes with static types, checked integer arithmetic, resource limits, and atomic failure. | No lexical scopes, capture, recursion, nested calls, arbitrary arity, general returns, or target/ABI execution. |
| **Measured feedback provenance** | U4 retains a named measurement result through one adjacent direct-`x` feedback shape and locally enumerates its two branches. | No generic branch execution, `else`, loops, multiple-result lifetime, or sampled host value. |
| **Foreign-consumer proof instead of wrapper marketing** | C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend consumers exercise the opaque C ABI in the Ubuntu full profile. | No released language packages, source compatibility, bidirectional translation, frozen ABI, or broad platform-delivery guarantee. |
| **Evidence-gated release posture** | The current seven-job matrix covers ordinary Linux, Windows, macOS, three clean SDK consumers, and a narrow Linux/Clang ASan/UBSan core profile. | The sanitizer profile is not a security certification, fuzzing result, complete coverage metric, or all-platform sanitizer claim. |

## SynQ-native usage policy

### What users should write

All learning materials that demonstrate a supported SynQ program must use a
`synq` code block and a source form in the documented bounded subset. The
following is a valid shape for the verified U6 local evaluator:

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]
fn add(left: Integer, right: Integer) -> left + right

let total: Integer = add(20, 22)
```

This source is evaluated only through the explicit local mode and its stated
resource limits. It is not a claim that the function lowers to OpenQASM, runs
through ABI v1, or executes on a device. See
[`BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md`](./BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md).

### What remains intentionally non-SynQ

The phrase “use SynQ, not Python” cannot truthfully mean that every file in a
language project is written in SynQ. A compiler needs an implementation host
language; the verified SynQ recovery compiler is C++17. The external
OpenQASM reference-parser and Qiskit-import checks are Python test tools, not
SynQ execution. C, Rust, Common Lisp, Clojure, and Mercury are retained only as
explicit ABI-consumer conformance fixtures. Their purpose and limits are stated
in [`INTEROPERABILITY.md`](./INTEROPERABILITY.md).

| Material class | Allowed language use | Reader-facing rule |
| --- | --- | --- |
| Supported language examples and tutorials | **SynQ only** for programs presented as executable by `synqc`. | Do not substitute Python, Qiskit, Cirq, Q#, or invented syntax for a SynQ example. |
| Compiler implementation and CMake tests | C++ and CMake. | Describe these as implementation/test infrastructure, not SynQ programs. |
| OpenQASM compatibility validation | Python package tooling invoked by CTest. | Describe it only as downstream parser/import validation, never as SynQ runtime behavior. |
| ABI conformance fixtures | C, Rust, Common Lisp, Clojure, Mercury. | Label each as a foreign consumer proof and preserve its non-package boundary. |
| Historical Python, notebook, frontend, and backend material | Not part of the verified recovery compiler path unless a current document names a reproducible validation route. | Do not use it as language documentation or capability evidence. |

## Remediation completed by this audit

The historical Python-centric `TESTING.md`, the unsupported compiler manual and
reference, the unsupported keyring page, and the speculative backend “Phase 5”
document no longer teach a false language surface. They now direct readers to
the verified SynQ-native entry points and the current status ledger. The audit
does **not** assert that all historical source files are deleted or supported.

## Next evidence-producing work

The highest-value next technical addition is the already scheduled bounded
parser/runtime fuzz-replay target: a deterministic corpus, checked-in seeds,
reproduction commands, and failure classification. It strengthens the
auditable-boundedness direction without pretending to create a complete runtime.
The complementary [`KNOWN_LIMITS_v0.1.0.md`](./KNOWN_LIMITS_v0.1.0.md) is the
mandatory reader-facing guardrail for every such increment.

## References

[1]: https://www.ibm.com/quantum/qiskit "IBM Quantum: Qiskit"
[2]: https://learn.microsoft.com/en-us/azure/quantum/qsharp-overview "Microsoft Learn: Introduction to the Quantum Programming Language Q#"
[3]: https://quantumai.google/cirq "Google Quantum AI: Cirq"
[4]: https://docs.pennylane.ai/en/stable/ "PennyLane Documentation"
[5]: https://openqasm.com/versions/3.0/intro.html "OpenQASM 3.0 Specification: Introduction"
[6]: https://silq.ethz.ch/ "ETH Zürich: What is Silq?"
