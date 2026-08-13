# SynQ Bounded Quantum Gate-Shape Validation

**Status:** Remotely validated recovery-profile semantic boundary.
**Last reviewed:** 13 August 2026

## Purpose

The typed quantum AST records gate kind, literal angle, and numeric operands.
Before this increment, known gate arity and parameter-shape restrictions are
enforced only by the OpenQASM 3 exporter. That means an invalid known quantum
statement can parse successfully and fail later at a backend-specific boundary.

This increment implements a small compiler-neutral validation pass for only the
known recovery-profile gate shapes. It is a safety boundary, not a quantum
semantics engine, hardware validator, type checker, or optimizer.

> **Design rule:** reject malformed uses of gates whose bounded shape SynQ
> already claims to understand. Preserve syntactically valid unknown gate names
> as `Unknown` so a future language extension can define them deliberately;
> current exporters continue to reject them rather than guessing an operation.

## Initial gate-shape registry

| Gate kind | Literal-angle rule | Operand rule | Parser result |
| --- | --- | --- | --- |
| `H`, `X`, `Y`, `Z` | No literal angle. | Zero or one explicit qubit. | A structured semantic error for an angle or more than one operand. |
| `Cx` | No literal angle. | Exactly two explicit qubits. | A structured semantic error otherwise. |
| `BellPair` | No literal angle. | Zero or exactly two explicit qubits. | A structured semantic error otherwise. |
| `Rx`, `Ry`, `Rz`, `Phase` | A bounded literal angle is required. | Exactly one explicit qubit. | A structured semantic error otherwise. |
| `Unknown` | Retained exactly as parsed. | Retained exactly as parsed. | No speculative parser semantic error; bounded exporters diagnose it as unsupported. |

## Diagnostic namespace

The existing `SYNQ-P` registry describes syntax and parser-config failures.
This pass adds the `SYNQ-S` namespace for recovery-profile semantic shape
failures, reported through the same typed diagnostic model and C ABI formatting.

| Code | Condition | Default remediation |
| --- | --- | --- |
| `SYNQ-S001` | A known non-parameterized gate has a literal angle. | Remove the angle or use the documented parameterized gate form. |
| `SYNQ-S002` | A known non-parameterized gate has an unsupported number of explicit operands. | Match the documented gate arity. |
| `SYNQ-S003` | A known parameterized gate lacks a literal angle or does not have exactly one explicit operand. | Use a supported literal angle and one `q[index]` operand. |

The parser supplies the source span because it owns the original source line.
The validator supplies only a deterministic code, message, and remediation. This
keeps the validator independent of file I/O, parser cursor state, backends, and
external runtime state.

## Compatibility boundary

The pass now runs only after current recovery syntax has formed a
`QuantumGateNode`. Syntax errors and feature-gate errors retain their existing
`SYNQ-P` code. Existing supported source continues to parse. Direct construction
of `QuantumGateNode` remains possible for tests and future internal passes; the
OpenQASM exporter retains its own defensive arity checks because callers can
bypass the parser.

No semantic meaning is assigned to an `Unknown` gate, no automatic gate rewrite
is performed, and no provider, simulator, or hardware API is introduced.

## Exit criteria

The pure validator now covers all initial known gate classes; parser failures
return the documented `SYNQ-S` code with existing structured diagnostic fields;
unknown gates remain parseable but exporter-rejected; and the compiled C ABI
consumer checks propagation of `SYNQ-S002`. Compiler, frontend, and backend
suites pass locally. Status material calls this **bounded gate-shape
validation**, not general quantum semantic validation. The same compiler
profile passed in [Compiler Core #12][1] for commit `aae8e78`.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31721517239 "SynQ Compiler Core #12"
