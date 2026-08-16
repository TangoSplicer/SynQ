# Changelog

All notable changes to the SynQ experimental hybrid quantum-classical language repository will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to Semantic Versioning for its experimental CLI and C ABI contracts.

---

## [Unreleased]

### Added
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
- Add non-Ubuntu or alternate-toolchain clean-install evidence before making any
  cross-platform SDK claim.
- Design the resource and bounded-execution contracts required before
  multi-register simulation, non-literal control, or callable bodies/calls.

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
