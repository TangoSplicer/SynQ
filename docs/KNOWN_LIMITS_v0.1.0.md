# SynQ Known Limits Ledger v0.1.0

**Status:** Versioned boundary ledger for the experimental recovery profile.  
**Issued:** 20 August 2026.  
**Evidence baseline:** [Compiler Core #32403435066](https://github.com/TangoSplicer/SynQ/actions/runs/32403435066), revision `8125287`.

> This ledger records what SynQ **does not** currently claim. A future capability
> may be added only when it has a written contract, focused local tests, the full
> required local recovery result, and a passing remote Compiler Core matrix for
> the same revision. It is not a feature roadmap or a promise of delivery.

## Interpretation

SynQ is an experimental hybrid quantum–classical language kernel, not a
feature-complete language, provider client, hardware service, or stable SDK.
The verified profile is a deliberately bounded C++17 compiler and CLI. The
baseline has 48/48 ordinary Ubuntu recovery CTests, 34/34 Windows MSVC and
macOS Clang platform-neutral CTests, three static-SDK consumer jobs, and a
narrow 33/33 Linux/Clang ASan/UBSan core profile. It also has a five-source,
eight-seed deterministic replay smoke target, plus a separately verified
non-publishing candidate archive/manifest/checksum/extraction/attestation
workflow with a validated optional existing-tag input. These are additive reliability evidence, **not** a security
certification, continuous fuzzing service, public-package release, or a full
fuzzing result.

## Language and semantic limits

| Area | Present verified boundary | Not currently claimed |
| --- | --- | --- |
| Surface grammar | Only documented, feature-gated U1–U6 forms and their named fixtures are part of the current recovery subset. | The whole tracked `.synq` corpus is not automatically a supported language standard. |
| Types and bindings | Bounded top-level immutable values; Alpha fixed-type mutable cells; bounded one-/two-formal callable shapes. | General type system, algebraic types, generics, collections, modules, traits, strings beyond stated unary local-call coverage, or lexical scope semantics. |
| Classical execution | Bounded `--eval-constants`, `--eval-state`, and U5/U6 `--eval-runtime` with explicit depth/operation/cell/invocation limits. | General evaluator, capture, nested calls, recursion, arbitrary arity, mixed-type binary calls, mutable locals, general returns, exception model, concurrency, or I/O. |
| Quantum routines | U3 source-only static expansion within documented declaration/call limits. | Routine execution, dynamic parameters, capture, recursion, nested calls, allocation, or target-side routine definitions. |
| Measurement and control | U4 accepts one named measurement followed immediately by one direct `x` feedback form; its local model enumerates two branches. | Sampled host values, general collapse API, `else`, loops, multiple-result lifetimes, general branch execution, or feedback in routine bodies. |
| Resource model | Declared named registers and bounded declaration-order local simulation. | Dynamic allocation, resource lifetime, aliasing, deallocation, ownership model, physical mapping, scheduling, or timing semantics. |

## Execution, target, and hardware limits

| Area | Present verified boundary | Not currently claimed |
| --- | --- | --- |
| Local simulation | Opt-in pure-state probability calculation for a limited gate/register subset; one U4 feedback pair through deterministic enumeration. | Noise, sampling API, general measurements, arbitrary gates, hardware fidelity, provider behavior, or performance guarantees. |
| OpenQASM | Strict Hybrid export for explicitly mapped constructs, with exact fixtures and downstream parser/import checks. | Full OpenQASM support, import, execution, semantic equivalence outside the tested subset, provider integration, or hardware submission. |
| CLI | `--validate`, strict export modes, semantic inspection, bounded evaluation modes, and limited simulation. | Stable CLI, package manager, REPL, project scaffolding, debugger, cloud workflow, daemon, or IDE integration. |
| Hardware and services | None. | Quantum-hardware access, cloud accounts, job submission, provider service, SLAs, hosted compiler, availability guarantee, or enterprise support. |

## ABI, platform, and interoperability limits

| Area | Present verified boundary | Not currently claimed |
| --- | --- | --- |
| Native SDK | Experimental static library/header/CMake package consumer proof on Ubuntu 22.04, Windows MSVC, and macOS Clang. | Frozen ABI, shared-library distribution, general binary compatibility, registry package, or broad cross-platform support. |
| ABI consumers | Ubuntu full-profile C, Rust, test-only Common Lisp/CFFI, test-only Clojure/JNA, and test-only Mercury C-backend proofs. | Released bindings, source-language interoperability, bidirectional translation, general host-language APIs, or platform-wide fixture coverage. |
| Rust wrapper | Source-only Alpha `synq-alpha` adapter with three Cargo conformance tests in the stated Ubuntu path. | Registry publication, stable API, shared distribution, or a complete Rust language integration. |

## Evidence and security limits

| Area | Present verified boundary | Not currently claimed |
| --- | --- | --- |
| Test evidence | Named smoke fixtures, a conformance fixture manifest, a U1–U6 requirement matrix, ordinary platform tests, SDK consumers, a fixed replay corpus/schedule, and a narrow sanitizer profile. | Complete coverage, absence of defects, continuous fuzzing coverage, property testing, formal verification, security certification, or a complete conformance suite. |
| Sanitizers | Opt-in Linux/Clang ASan/UBSan applies to an isolated 33-test core profile with stated halt/leak settings, including deterministic replay. | Sanitizer results for Windows, macOS, SDK consumers, all interop fixtures, external parsers, production deployments, or every source path. |
| Reproducibility | Exact build/test commands, corpus source names, replay seeds, a three-platform candidate archive workflow, checksums/manifests, and remote run links are documented for current evidence. | Hermetic build reproduction on every machine, public release assets, signed release artifacts, notarization, package-manager delivery, or arbitrary-input fuzz replay. |

## Documentation and historical-material limits

The repository contains historical `.synq`, Python, notebook, backend, frontend,
and compiler-adjacent material outside the current recovery profile. Source
presence is not proof of compiler support. The root README, `PROJECT_STATUS.md`,
the bounded-feature contracts, the conformance documents, and this ledger take
precedence for current capability statements. Historical material may be useful
for archaeology, but it must not be used as a language specification or product
promise.

## Promotion rule

A proposed capability may move from this ledger into a verified surface only if
all of the following are true for the same revision:

1. A concise contract states accepted syntax, semantics, limits, diagnostics,
   target/ABI behavior, and explicit non-goals.
2. Positive, negative, boundary, and regression fixtures cover the contract.
3. The required ordinary local recovery profile passes.
4. The required remote Compiler Core jobs pass.
5. `README.md`, `PROJECT_STATUS.md`, the changelog, conformance records, and
   public Evidence Ledger are aligned before any broader claim is made.

## Related records

See [`PROJECT_STATUS.md`](./PROJECT_STATUS.md),
[`RUNTIME_READINESS_GAP_ASSESSMENT_2026-08-19.md`](./RUNTIME_READINESS_GAP_ASSESSMENT_2026-08-19.md),
[`RUNTIME_SUBSET_CLAIM_STANDARD.md`](./RUNTIME_SUBSET_CLAIM_STANDARD.md),
[`CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md`](./CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md),
and [`UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md`](./UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md).
