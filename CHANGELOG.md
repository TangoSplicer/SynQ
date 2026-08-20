# Changelog

All notable changes to the SynQ experimental hybrid quantum-classical language repository will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to Semantic Versioning for its experimental CLI and C ABI contracts.

---

## [Unreleased]

### Added
- **Prepared experimental release-readiness records:** Added a proposed-tag
  record for `v0.1.0-experimental`, tied to the exact remotely verified
  candidate revision and retained CI archive hashes without creating the tag or
  a GitHub Release. Added a user-facing archive-verification guide and a
  reusable pre-release notes template with explicit SHA-256, optional
  release-integrity, GitHub-attestation, extraction, experimental-boundary, and
  final-confirmation checks. These records prepare a future release only; no
  public asset, signing/notarization, installer, package-manager package, or
  support commitment has been created.
- **Non-publishing synqc candidate archives:** Added a portable CMake
  candidate packager and extraction smoke target that stages only `synqc`,
  licenses, current bounded-profile documentation, a notice, and version JSON;
  writes a SHA-256 manifest; rejects all non-allowlisted archive entries; and
  validates the extracted CLI through `--version`, `--help`, and a verified
  SynQ validation fixture. The `synqc Release Candidate` workflow has no
  release-publication permission and passed Linux, macOS ARM64, and Windows X64
  archive, extraction, GitHub provenance-attestation, and CI-artifact-retention
  jobs in [run #32393670587](https://github.com/TangoSplicer/SynQ/actions/runs/32393670587).
  The exact revision `fdbbbec` passed all seven Compiler Core jobs in
  [run #32394772652](https://github.com/TangoSplicer/SynQ/actions/runs/32394772652):
  48/48 ordinary Linux, 34/34 Windows MSVC/macOS Clang, three static-SDK consumer,
  and 33/33 Linux/Clang sanitizer CTests. The retained candidate artifacts are
  not public releases, signed/notarized binaries, installers, or registry
  packages; publication still requires user confirmation and separate release
  metadata.
- **Deterministic parser/runtime replay and native-usage audit:** Added the
  `synq_deterministic_replay_smoke` C++17 recovery target with a checked-in
  five-source SynQ corpus, fixed eight-seed single-byte mutation schedule,
  4 KiB input cap, in-memory parser/Hybrid-IR/resolver/bounded-evaluator
  pipeline, twice-per-input outcome-digest comparison, and an exact
  `--case`/`--seed` reproduction interface. The target is registered in both
  the ordinary and isolated sanitizer profiles. [Compiler Core #32374149046](https://github.com/TangoSplicer/SynQ/actions/runs/32374149046)
  passed all seven jobs for revision `8fc1de5`: 47/47 ordinary Linux CTests,
  33/33 Windows MSVC and macOS Clang platform-neutral CTests, three
  clean-install static-SDK consumer jobs, and 33/33 Linux/Clang ASan/UBSan core
  CTests. This is fixed deterministic replay—not continuous fuzzing, libFuzzer,
  property-test coverage, a general-runtime proof, or a security certification.
  The same revision adds a cited differentiation and SynQ-native usage audit,
  a versioned known-limit ledger, current testing guidance, and retirement
  notices for unsupported Python-centric or invented language documentation.
- **Zero-cost Clang sanitizer core profile:** The opt-in
  `SYNQ_ENABLE_SANITIZERS=ON` CMake profile uses Clang AddressSanitizer and
  UndefinedBehaviorSanitizer with frame pointers, non-recovering undefined
  behavior diagnostics, leak detection, and halt-on-error settings. An additive
  Linux Compiler Core job configures, builds, and runs the isolated core profile
  without changing the established ordinary Linux, Windows, macOS, or static-SDK
  jobs. [Compiler Core #32369872013](https://github.com/TangoSplicer/SynQ/actions/runs/32369872013)
  passed all seven jobs for revision `2965ec4`: 32/32 sanitizer CTests, 46/46
  ordinary Linux recovery CTests, 32/32 Windows MSVC and macOS Clang
  platform-neutral CTests, and three clean-install static-SDK consumer jobs. The
  sanitizer evidence is limited to that Linux/Clang 32-test profile; it does not
  add fuzzing, property testing, deterministic replay, a security audit, a
  production-hardening mode, a stable runtime, or hardware capability claim.
- **Alpha bounded binary classical callable runtime (U6):** The jointly gated
  `classical-callable-execution` and `multi-formal-classical-callables` subset
  accepts one earlier `fn name(left: Integer|Boolean, right: same-type) ->`
  documented binary body and one immutable ordered two-actual invocation.
  Typed AST/Hybrid IR and resolution retain callable, each formal/actual, exact
  static type, and earlier-definition provenance. `synqc --eval-runtime`
  evaluates a non-capturing two-binding depth-1 local frame with checked `int64`
  arithmetic, deterministic Boolean operators, and the existing
  32-declaration/128-invocation/depth-16/128-operation limits; failure is
  atomic. Strict Hybrid export, bounded quantum simulation, and ABI v1
  explicitly reject U6 nodes. A focused U6 fixture, CLI smoke extension, C ABI
  rejection check, parser/resolver/boundary/overflow/limit tests, and
  beginner-first tutorial are included. [Compiler Core platform-matrix run
  #32270327206](https://github.com/TangoSplicer/SynQ/actions/runs/32270327206)
  passed all six jobs for revision `9a1a9b3`: 46 Linux CTests, 32 Windows MSVC
  CTests, 32 macOS Clang CTests, and clean-install experimental static-SDK
  external-consumer checks on Ubuntu 22.04, Windows MSVC, and macOS Clang. This
  adds no arbitrary arity, mixed types, String binary body, capture, mutable
  local state, returns, recursion, nested calls, target execution, provider,
  network, AI, or hardware claim.
- **Alpha bounded classical callable runtime (U5):** The feature-gated
  `classical-callable-execution` subset accepts one earlier
  `fn name(value: Integer|Boolean|String) ->` parameter-only body and one
  immutable `let result = name(actual)` invocation. Typed AST/Hybrid IR and
  resolution retain function, actual, exact type, and earlier-definition
  provenance. `synqc --eval-runtime` evaluates one local depth-1 frame with
  checked `int64` arithmetic and 32-declaration/128-invocation/depth-16/
  128-operation limits; failure is atomic. Strict Hybrid export, bounded quantum
  simulation, and ABI v1 explicitly reject U5 nodes. Parser, resolver, evaluator,
  CLI, C ABI, boundary, limit, tutorial, and compatibility fixtures are included.
  [Compiler Core platform-matrix run #32266056516](https://github.com/TangoSplicer/SynQ/actions/runs/32266056516)
  passed all six jobs for revision `7307c00`: 45 Linux CTests, 31 Windows MSVC
  CTests, 31 macOS Clang CTests, and clean-install experimental static-SDK
  external-consumer checks on Ubuntu 22.04, Windows MSVC, and macOS Clang. This
  adds no general runtime, capture, mutable local state, multi-argument calls,
  returns, recursion, nested calls, target execution, provider, network, AI, or
  hardware claim.
- **Alpha bounded measurement feedback (U4):** The feature-gated
  `measurement-feedback` subset accepts one top-level `measure register[index]
  as result` immediately followed by one direct `if result then quantum x
  register[index]`. Typed AST/Hybrid IR retain opt-in result provenance;
  resolution creates one terminal feedback node and rejects non-direct,
  non-adjacent, reused, invalid, and post-feedback shapes. Strict Hybrid export
  emits one exporter-owned scalar `bit`, one measurement assignment, and one
  conditional `x`. The bounded simulator enumerates the zero/one measurement
  branches deterministically and returns weighted final probabilities without a
  sampled host value. ABI v1 explicitly rejects U4 nodes. Parser/IR/resolver/
  exporter/CLI/reference-parser/simulator/C-ABI/limit and compatibility fixtures
  are included. [Compiler Core platform-matrix run #32250265354](https://github.com/TangoSplicer/SynQ/actions/runs/32250265354)
  passed all six jobs for revision `62baf18`: 44 Linux CTests, 30 Windows MSVC
  CTests, 30 macOS Clang CTests, and clean-install experimental static-SDK
  external-consumer checks on Ubuntu 22.04, Windows MSVC, and macOS Clang. This
  adds no general measurement API, sampled host result, `else`, loop, general
  control execution, user target-side state, routine feedback, provider, or
  hardware claim.
- **Alpha bounded parameterized quantum routines (U3):** The feature-gated
  `parameterized-quantum-routines` subset accepts exactly three one-gate kernel
  signatures: literal-angle plus one qubit for `rx`/`ry`/`rz`/`p`, one qubit for
  `h`/`x`/`y`/`z`, or two qubits for `cx`. Typed AST and Hybrid IR retain ordered
  formals, symbolic body operands, actuals, and spans. Resolution enforces
  earlier definitions, exact ordered kinds/arity, declared in-range actuals, and
  distinct two-qubit resources. Strict Hybrid OpenQASM expands one static gate per
  call with 32-declaration/128-call limits; the simulator and C ABI explicitly
  reject U3 nodes. The implementation adds parser, AST/IR, resolver, exact-output,
  CLI, reference-parser, simulator, ABI, limit, and zero-parameter compatibility
  fixtures. [Compiler Core platform-matrix run #32247154982](https://github.com/TangoSplicer/SynQ/actions/runs/32247154982)
  passed all six jobs for revision `be18e0f`: 41 Linux CTests, 28 Windows MSVC
  CTests, 28 macOS Clang CTests, and clean-install experimental static-SDK
  external-consumer checks on Ubuntu 22.04, Windows MSVC, and macOS Clang. This
  introduces no routine execution, target-side routine definition, capture,
  allocation, recursion, nested calls, runtime angle, hardware, or provider claim.
- **Alpha bounded mutable classical state:** The feature-gated top-level
  `var`/`set` subset now has typed AST and Hybrid IR nodes, sequential resolver
  checks, deterministic local-only `synqc --eval-state`, final cell declaration/
  last-write provenance, and default bounds of 64 cells, 128 transitions, 16
  expression depth, and 128 Boolean/Integer operations. It accepts only fixed
  Boolean/Integer/String cell types and earlier references with exact write
  types. `SYNQ-P014`–`SYNQ-P016`, `SYNQ-S005`–`SYNQ-S007`, and
  `SYNQ-E008`–`SYNQ-E010` cover the gated syntax, resolver, opt-in, bound, and
  unsupported-node paths. Strict Hybrid OpenQASM and bounded quantum simulation
  reject state nodes; no branch execution, target storage, measurement feedback,
  ABI state execution, or general runtime is introduced. [Compiler Core
  platform-matrix run #32242711770](https://github.com/TangoSplicer/SynQ/actions/runs/32242711770)
  passed all six jobs: 38 Linux CTests, 27 Windows MSVC CTests, 27 macOS Clang
  CTests, and clean-install experimental static-SDK external-consumer checks on
  Ubuntu 22.04, Windows MSVC, and macOS Clang.
- **Alpha Boolean declaration-expression evaluation:** `synqc --eval-constants`
  now evaluates resolved immutable top-level Boolean literal/reference/`not`/
  `and`/`or` trees deterministically. The implementation applies explicit
  default expression-depth (`16`) and request-wide operation (`128`) budgets,
  reports exhaustion through `SYNQ-E006`/`SYNQ-E007`, and does not introduce
  short-circuit, assignment, mutable state, branch execution, target-side
  expression execution, or measurement-result semantics. Parser, typed IR,
  resolver, evaluator, CLI, positive/negative budget, and full matrix coverage
  are included. [Compiler Core platform-matrix run
  #32239066421](https://github.com/TangoSplicer/SynQ/actions/runs/32239066421)
  passed all six jobs: 37 Linux CTests, 26 Windows MSVC CTests, 26 macOS Clang
  CTests, and clean-install experimental static-SDK external-consumer checks on
  Ubuntu 22.04, Windows MSVC, and macOS Clang.
- **Bounded callable-kernel source lowering:** Strict Hybrid OpenQASM 3 now
  accepts one earlier non-recursive, zero-parameter `kernel` with exactly one
  non-parameterized default-register gate body and one later `call`, expanding
  the typed gate at source-generation time. Parser, Hybrid IR, resolver, exact
  output, CLI emission, and OpenQASM reference-parser coverage are included.
  [Compiler Core platform-matrix run #32191872033](https://github.com/TangoSplicer/SynQ/actions/runs/32191872033)
  passed all five jobs: 37 Linux CTests and 26 Windows/macOS platform-neutral
  CTests. Functions, parameters, returns, named-register operands, recursion,
  nested calls, control/measurement bodies, simulation, ABI execution, and
  runtime dispatch remain unsupported.
- **Compile-time literal-negation `if` lowering:** Strict Hybrid OpenQASM 3 now
  folds `if not true` to `if (false)` and `if not false` to `if (true)` for one
  supported typed gate body, without generated target Boolean storage. Exact
  output, CLI emission, and OpenQASM reference-parser coverage are included.
  [Compiler Core platform-matrix run #32188899985](https://github.com/TangoSplicer/SynQ/actions/runs/32188899985)
  passed all five jobs: 35 Linux CTests and 25 Windows/macOS platform-neutral
  CTests. Nested negation, binary expressions, aliases, measurement results,
  loops, and local branch execution remain unsupported.
- **Bounded negated identifier-`if` target lowering:** Strict Hybrid OpenQASM 3
  now lowers `if not <earlier Boolean-literal declaration>` for one supported
  typed gate body by reusing immutable generated target Boolean storage. Exact
  output, negative boundaries, CLI emission, and OpenQASM reference-parser
  coverage are included. [Compiler Core platform-matrix run
  #32075281245](https://github.com/TangoSplicer/SynQ/actions/runs/32075281245)
  passed all five jobs: 33 Linux CTests and 24 Windows/macOS platform-neutral
  CTests. Negated Boolean literals, binary expressions, aliases, measurement
  results, loops, and local branch execution remain unsupported.
- **Bounded identifier-`if` target lowering:** Strict Hybrid OpenQASM 3 now
  lowers one `if` gate body controlled by an earlier top-level Boolean-literal
  declaration into generated immutable target Boolean storage. Exact-output,
  negative-boundary, CLI, and OpenQASM reference-parser tests are included.
  [Compiler Core platform-matrix run #32069791226](https://github.com/TangoSplicer/SynQ/actions/runs/32069791226)
  passed all five jobs: 31 Linux CTests and 23 Windows/macOS platform-neutral
  CTests. Boolean expressions, aliases, measurement results, loops, and local
  branch execution remain unsupported.
- **Source-only Alpha Rust wrapper:** Added the dependency-free `synq-alpha`
  Cargo package over the opaque C ABI, with RAII ownership, source parsing,
  OpenQASM 3 export, ABI identity access, and three Cargo conformance tests.
  [Compiler Core platform-matrix run #32067123839](https://github.com/TangoSplicer/SynQ/actions/runs/32067123839)
  passed all five jobs, including the 29-test Ubuntu full profile that executes
  the wrapper tests. This does not publish a registry package, distribute a
  shared library, or stabilize a Rust API.
- **Bounded multi-register simulation:** Added deterministic declaration-order
  physical allocation for explicit named registers, cross-register supported
  gates, source-register measurement provenance, combined qubit-limit rejection,
  and `synqc --simulate` register-offset output. [Compiler Core platform-matrix
  run #32065348941](https://github.com/TangoSplicer/SynQ/actions/runs/32065348941)
  passed all five jobs.
- **Inspectable Alpha semantic environment:** Added read-only resolved
  top-level binding metadata, deterministic `synqc --inspect-semantics` output,
  an explicit semantic-kernel contract, and focused resolver/CLI fixtures.
  [Compiler Core platform-matrix run #32063787617](https://github.com/TangoSplicer/SynQ/actions/runs/32063787617)
  passed 28 Linux CTests and 21 Windows/macOS platform-neutral CTests.
- **Expanded platform evidence:** [Compiler Core platform-matrix run #32018610062](https://github.com/TangoSplicer/SynQ/actions/runs/32018610062)
  passed independent Linux 27-test, Windows MSVC 20-test, macOS Clang 20-test,
  Ubuntu 22.04 static-SDK consumer, and Windows MSVC static-SDK consumer jobs.
  Each artifact/job remains separately scoped; macOS SDK conformance is not
  claimed.
- **Language-wide improvement assessment:** Added a prioritized semantic,
  resource, control-flow, callable, interoperability, and release-contract
  assessment with explicit dependency order and current non-claims.
- **Separate Windows and Linux CI artifacts:** Added independent Linux and
  Windows build/smoke jobs with separately named artifacts. [Compiler Core
  multi-platform run #31976468444](https://github.com/TangoSplicer/SynQ/actions/runs/31976468444)
  passed the Linux 27-test recovery profile, the Windows MSVC 20-test
  platform-neutral profile, and the distinct Ubuntu 22.04 static-SDK consumer
  job.
- **Second clean-install environment:** Added a fixed Ubuntu 22.04 producer,
  install, discovery, external-consumer, and execution job. Together with the
  existing `ubuntu-latest` recovery profile, it passed in [Compiler Core #50](https://github.com/TangoSplicer/SynQ/actions/runs/31956231719).
- **Beginner experience:** Added a beginner tutorial, non-executing guided CLI
  walkthrough contract, and visual Bell-state lesson design record.
- **Competitive review:** Added a cited, evidence-bound comparison of SynQ’s
  current kernel against relevant quantum-programming ecosystems.
- **Tested-environment guide:** Published the verified Ubuntu-like compiler and
  clean-install boundary without claiming a broader support matrix.
- **Alpha named-register operands:** Added parser, Hybrid IR, resolver, range
  diagnostics, strict-Hybrid OpenQASM lowering, CLI coverage, and explicit
  simulator rejection. [Compiler Core #47](https://github.com/TangoSplicer/SynQ/actions/runs/31951911553)
  passed **27/27** checks.
- **Strict literal-if gate lowering:** Added the exact `if true/false then
  quantum <supported gate>` OpenQASM source-lowering subset, CLI coverage, and
  rejection fixtures for wider control forms. [Compiler Core #48](https://github.com/TangoSplicer/SynQ/actions/runs/31952214849)
  passed **27/27** checks.
- **Community and contract materials:** Added factual interoperability guidance,
  contribution workflow, security-reporting policy, and bounded issue templates.

### Changed
- **Evidence baseline and teaching materials:** Updated the README, project
  status, runtime-gap assessment, testing guide, deterministic replay contract,
  and known-limit ledger to the #32374149046 evidence baseline. Learner-facing
  SynQ programs use SynQ syntax; C, Rust, Mercury, Common Lisp, Clojure, and
  Python remain only where explicitly identified as implementation, ABI-consumer,
  or downstream-validation tooling.
- **Portable SDK conformance harness:** Made the isolated SDK producer/consumer
  script configuration-aware so it can select a multi-configuration build,
  executable suffix, and vcpkg toolchain for the verified Windows MSVC path.
- **MSVC portability and smoke fixtures:** Replaced GNU-only checked-arithmetic
  and `M_PI` assumptions, separated optional Ubuntu toolchain fixtures from the
  Windows profile, and made parser, diagnostics, exporter, C ABI, and CLI smoke
  fixtures portable across the validated CI runners. The Windows job is not an
  SDK or language-wrapper compatibility claim.
- **Older JSON compatibility:** Replaced an implicit JSON-to-vector assignment
  with an explicit typed conversion so the static SDK producer builds under the
  older nlohmann-json package in the fixed Ubuntu 22.04 job.
- **Repository status documents:** Replaced stale compiler, CLI, security,
  support, and release assertions with the current experimental recovery-profile
  evidence and explicit non-claims.

### Planned
- Define wider callable semantics only after scope, return, resource-effect,
  recursion, and execution contracts are independently specified and tested.
- Define any expansion beyond one U4 feedback pair only after multiple-result
  lifetime, `else`, general-control, and target/runtime contracts are separately
  specified and tested.

---

## [0.1.0-experimental] - 2026-08-15

### Added
- **Compiler Core Recovery Profile (#1–#46):** Built and remotely validated via GitHub Actions (`https://github.com/TangoSplicer/SynQ/actions`).
- **Bounded Boolean & Integer Arithmetic Expressions:** Added parser, semantic validation, and evaluation support for constrained classical expressions.
- **Qubit Declarations & Allocation Tracking:** Added default-register and qubit extent validation (`q[n]`).
- **Typed Measurement Results & Hybrid IR OpenQASM 3 Export:** Added exact lowering and external parser/Qiskit import validation.
- **Callable Declarations & Bounded Constants:** Added declaration-only `fn`/`kernel` parsing and `evaluate_bounded_constants`.
- **Bounded Local Quantum Simulator:** Added pure-state vector simulation for up to 10 qubits and 1024 operations supporting H, X, Y, Z, CX, Bell pair, RX, RY, RZ, and phase gates.
- **Experimental Native SDK Path:** Added explicit header installation (`synq_ffi.h`), static library (`libsynq_lib.a`), and `SynQ::synq_lib` CMake package export with an isolated clean-prefix conformance test (passing 27/27 CTest checks).
- **Public Evidence Ledger Showcase:** Rebuilt the React/Vite public preview website (`synqshowcase-ee5qsvs3.manus.space`) around traceable recovery evidence, explicit non-claims, and the published ABI v1 policy.

### Changed
- **CLI Versioning:** Maintained `synqc 0.1.0-experimental` as the truthful version for the bounded recovery CLI artifact.
- **Documentation Alignment:** Replaced obsolete speculative v1.0.0 release notes with a factual experimental engineering changelog.
