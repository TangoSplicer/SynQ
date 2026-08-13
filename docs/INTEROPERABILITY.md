# SynQ Interoperability Boundary

**Status:** The first adapter is implemented and passes its local fixture-based recovery-profile smoke test. A remote compiler-core CI result must be observed for the publication revision before a CI-backed claim is made.

> SynQ does not currently provide general-purpose source compatibility, bidirectional translation, package interoperability, or hardware-provider execution. This document defines the first deliberately narrow compatibility boundary that can be tested within the compiler recovery profile.

## Selected First Adapter: OpenQASM 3 Export

The first adapter exports a **small quantum-kernel subset** from the recovered SynQ AST to textual **OpenQASM 3.0**. OpenQASM is designed to act as an intermediate representation through which higher-level compilers communicate quantum programs to downstream tooling and hardware-oriented systems [1]. Its standard library exposes `stdgates.inc` and the gate vocabulary needed for a minimal portable circuit subset [2].

| SynQ recovery input | Planned OpenQASM 3 output | Boundary |
|---|---|---|
| `quantum h` | `h q[0];` | One-qubit Hadamard fixture only. |
| `quantum x`, `quantum y`, `quantum z` | Corresponding single-qubit standard gate | Exact named gate subset only. |
| `quantum bell_pair` | `h q[0];` followed by `cx q[0], q[1];` | Fixed two-qubit Bell-pair fixture only. |
| `let`, `print`, `delay`, `ai`, unknown kernel | Rejected with a line-specific diagnostic | No semantic translation is claimed. |

Every successful export emits `OPENQASM 3.0;`, includes `stdgates.inc`, and allocates only the number of qubits required by the supported kernel. An unsupported statement makes the export fail rather than silently dropping behavior or manufacturing a translation. The `synq_openqasm3_exporter_smoke` fixture checks exact output, parser-to-exporter ordering, allocation, and diagnostic behavior for rejected declarations and unsupported instructions.

## Explicit Non-Goals

This first adapter will **not** import OpenQASM, execute OpenQASM, invoke Qiskit or any other SDK, submit to a provider, validate output with an external OpenQASM parser, translate general classical code, infer qubit counts, or translate the older unverified Qiskit backend. It establishes a testable source-export seam only.

## Promotion Rule

A kernel enters the compatibility surface only after it has a SynQ parser fixture, an exporter fixture with exact expected output, a recovery-profile build, and a passing compiler-core CI run.

## References

[1]: https://openqasm.com/versions/3.0/intro.html "OpenQASM 3.0 Specification — Introduction"
[2]: https://openqasm.com/language/standard_library.html "OpenQASM Live Specification — Standard library"
