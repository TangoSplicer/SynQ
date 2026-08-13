# SynQ Interoperability Boundary

**Status:** The bounded OpenQASM adapter remains the verified source-export boundary. The C ABI foundation and typed measurement export both have remote compiler-core evidence. See [C ABI Foundation](./C_ABI.md).

> SynQ does not currently provide general-purpose source compatibility, bidirectional translation, package interoperability, or hardware-provider execution. This document defines the first deliberately narrow compatibility boundary that can be tested within the compiler recovery profile.

## Selected First Adapter: OpenQASM 3 Export

The first adapter exports a **small quantum-kernel subset** from the recovered SynQ AST to textual **OpenQASM 3.0**. OpenQASM is designed to act as an intermediate representation through which higher-level compilers communicate quantum programs to downstream tooling and hardware-oriented systems [1]. Its standard library exposes `stdgates.inc` and the gate vocabulary needed for a minimal portable circuit subset [2].

| SynQ recovery input | OpenQASM 3 output | Boundary |
|---|---|---|
| `quantum h [q[index]]`, likewise `x`, `y`, `z` | Corresponding single-qubit standard gate | The operand is optional only for the legacy `q[0]` fallback; more than one operand is rejected. |
| `quantum cx q[control], q[target]` | `cx q[control], q[target];` | Exactly two explicit operands are required. |
| `quantum bell_pair [q[first], q[second]]` | `h q[first];` followed by `cx q[first], q[second];` | Zero operands retains the legacy `q[0]`, `q[1]` fixture; otherwise exactly two explicit operands are required. |
| `quantum rx`, `ry`, `rz`, or `p` with a literal angle | Corresponding parameterized standard gate | Exactly one explicit operand and one literal angle are required. Accepted angle text is a decimal literal, `pi`, `-pi`, `pi/<positive integer>`, or `-pi/<positive integer>`. |
| `measure q[index]` | `c[index] = measure q[index];` and a matching `bit[n] c;` register | Exactly one explicit non-negative qubit operand is required. The bit register appears only when a measurement exists; SynQ does not expose its result. |
| `let`, `print`, `delay`, `ai`, malformed operands, unknown kernel | Rejected with a line-specific diagnostic | No semantic translation is claimed. |

Every successful export emits `OPENQASM 3.0;`, includes `stdgates.inc`, and allocates only the number of qubits required by the supported kernel. It allocates a matching classical-bit register only when a typed measurement exists. An unsupported statement makes the export fail rather than silently dropping behavior or manufacturing a translation. The `synq_openqasm3_exporter_smoke` fixture checks exact output, parser-to-exporter ordering, allocation, typed measurement lowering, and diagnostic behavior for rejected declarations, unsupported instructions, invalid operands, invalid gate arity, and unsupported parameter expressions. It writes a generated fixture that `synq_openqasm3_reference_parse` validates with the pinned `openqasm3[parser]` reference-package parser [3]. `synq_openqasm3_qiskit_import` independently imports the same fixture with `qiskit-qasm3-import==0.6.0` and checks its six-qubit, six-classical-bit, nine-operation `QuantumCircuit` shape [4]. These checks confirm parser acceptance and one downstream conversion path, not semantic equivalence, provider support, or hardware support.

## Explicit Non-Goals

This first adapter will **not** import OpenQASM, execute OpenQASM, invoke Qiskit or any other SDK, submit to a provider, translate general classical code, infer qubit counts, or translate the older unverified Qiskit backend. The emitted source is validated by an external OpenQASM parser and a Qiskit importer, but this does not make SynQ a runtime integration or provider adapter.

## Native C ABI Foundation

The initial native interoperability seam is a C header with opaque program
handles and explicit ownership functions. Its compiled C consumer verifies
version identification, feature-gated file and in-memory parsing, typed
measurement export, diagnostic ownership, and cleanup. A dependency-free Rust C ABI consumer now
compiles and runs remotely against that same contract, including in-memory text
parsing; it is a proof of call and
ownership flow, not a Rust binding. A test-only SBCL/CFFI consumer locally
loads the same C ABI from a build-directory shared library; it is not a
Common Lisp package or general binding. Mercury and JVM-facing work remain
unimplemented. The full C API is documented in [C ABI Foundation](./C_ABI.md);
the Rust proof and non-goals are documented in [Rust C-ABI Consumer Proof](./RUST_C_ABI.md),
and the Lisp proof is documented in [Common Lisp CFFI Consumer Proof](./COMMON_LISP_CFFI.md).

## Promotion Rule

A quantum source construct enters the compatibility surface only after it has a SynQ parser fixture, an exporter fixture with exact expected output, a recovery-profile build, and a passing compiler-core CI run.

## References

[1]: https://openqasm.com/versions/3.0/intro.html "OpenQASM 3.0 Specification — Introduction"
[2]: https://openqasm.com/language/standard_library.html "OpenQASM Live Specification — Standard library"
[3]: https://pypi.org/project/openqasm3/ "openqasm3 — Python Reference AST and parser"
[4]: https://qiskit.github.io/qiskit-qasm3-import/ "Qiskit OpenQASM 3 Importer API"
