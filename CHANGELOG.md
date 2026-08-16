# Changelog

All notable changes to the SynQ experimental hybrid quantum-classical language repository will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to Semantic Versioning for its experimental CLI and C ABI contracts.

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

---

## [Unreleased]

### Planned
- Named-register allocation and literal operand validation.
- Guarded `if` statement lowering in strict Hybrid IR.
- Expanded SDK clean-install coverage across target operating systems.
