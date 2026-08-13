# SynQ Interoperability Boundary

**Status:** The adapter passes local fixture-based recovery-profile smoke coverage and [Compiler Core #6](https://github.com/TangoSplicer/SynQ/actions/runs/31710413666) for revision `d2168de`, including the external OpenQASM reference parser. This remains a source-export claim only.

> SynQ does not currently provide general-purpose source compatibility, bidirectional translation, package interoperability, or hardware-provider execution. This document defines the first deliberately narrow compatibility boundary that can be tested within the compiler recovery profile.

## Selected First Adapter: OpenQASM 3 Export

The first adapter exports a **small quantum-kernel subset** from the recovered SynQ AST to textual **OpenQASM 3.0**. OpenQASM is designed to act as an intermediate representation through which higher-level compilers communicate quantum programs to downstream tooling and hardware-oriented systems [1]. Its standard library exposes `stdgates.inc` and the gate vocabulary needed for a minimal portable circuit subset [2].

| SynQ recovery input | OpenQASM 3 output | Boundary |
|---|---|---|
| `quantum h [q[index]]`, likewise `x`, `y`, `z` | Corresponding single-qubit standard gate | The operand is optional only for the legacy `q[0]` fallback; more than one operand is rejected. |
| `quantum cx q[control], q[target]` | `cx q[control], q[target];` | Exactly two explicit operands are required. |
| `quantum bell_pair [q[first], q[second]]` | `h q[first];` followed by `cx q[first], q[second];` | Zero operands retains the legacy `q[0]`, `q[1]` fixture; otherwise exactly two explicit operands are required. |
| `let`, `print`, `delay`, `ai`, malformed operands, unknown kernel | Rejected with a line-specific diagnostic | No semantic translation is claimed. |

Every successful export emits `OPENQASM 3.0;`, includes `stdgates.inc`, and allocates only the number of qubits required by the supported kernel. An unsupported statement makes the export fail rather than silently dropping behavior or manufacturing a translation. The `synq_openqasm3_exporter_smoke` fixture checks exact output, parser-to-exporter ordering, allocation, and diagnostic behavior for rejected declarations, unsupported instructions, invalid operands, and invalid gate arity. It writes a generated fixture that `synq_openqasm3_reference_parse` validates with the pinned `openqasm3[parser]` reference-package parser [3]. This checks parser acceptance, not semantic correctness or hardware support.

## Explicit Non-Goals

This first adapter will **not** import OpenQASM, execute OpenQASM, invoke Qiskit or any other SDK, submit to a provider, validate output with an external OpenQASM parser, translate general classical code, infer qubit counts, or translate the older unverified Qiskit backend. It establishes a testable source-export seam only.

## Promotion Rule

A kernel enters the compatibility surface only after it has a SynQ parser fixture, an exporter fixture with exact expected output, a recovery-profile build, and a passing compiler-core CI run.

## References

[1]: https://openqasm.com/versions/3.0/intro.html "OpenQASM 3.0 Specification — Introduction"
[2]: https://openqasm.com/language/standard_library.html "OpenQASM Live Specification — Standard library"
[3]: https://pypi.org/project/openqasm3/ "openqasm3 — Python Reference AST and parser"
