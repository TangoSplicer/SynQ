# SynQ Bounded Measurement Profile

**Status:** Locally verified recovery-profile quantum source-export boundary;
remote CI evidence for this increment is pending publication.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s bounded quantum profile can construct typed gates and emit a fixed
OpenQASM 3 gate subset. This increment adds a typed measurement source-export
statement, the smallest useful next quantum-language capability: it makes an
explicit quantum-to-classical boundary without introducing hardware execution,
result values, classical expressions, control flow, or runtime semantics.

> **Design rule:** a SynQ measurement records one explicitly indexed qubit and
> lowers it to a same-index classical bit in generated OpenQASM 3. SynQ does not
> claim to execute, return, store, or branch on that result.

## Bounded source syntax

```synq
quantum h q[0]
measure q[0]
```

| Source form | Parser result | Boundary |
| --- | --- | --- |
| `measure q[3]` | One typed `MeasurementNode` with qubit index `3`. | Supported recovery-profile syntax. |
| `measure q[-1]`, `measure q`, `measure q[0], q[1]` | A structured `SYNQ-P008` diagnostic. | Exactly one non-negative explicit qubit index is required. |
| `measure q[3] -> result` | A structured `SYNQ-P008` diagnostic. | No named storage, result variables, or arrow syntax exists yet. |
| `measure q[3]` at runtime | No runtime behavior is claimed. | This increment creates AST/source export only. |

`MeasurementNode` owns `qubit_index`, the parser’s `SourceSpan`, and its
legacy-compatible source line. It does not name a result because the current
language has neither a classical result type nor assignment/expression syntax.

## OpenQASM 3 lowering

When any measurement is present, the bounded exporter emits one classical
register matching the generated qubit-register size. A measurement on `q[i]`
lowers to `c[i] = measure q[i];`, preserving the explicit index and statement
order.

```openqasm
OPENQASM 3.0;
include "stdgates.inc";
qubit[1] q;
bit[1] c;
h q[0];
c[0] = measure q[0];
```

| Export concern | This increment does | This increment does not do |
| --- | --- | --- |
| Register allocation | Emits `bit[n] c;` only if at least one measurement exists; `n` remains the qubit extent. | Allocate user-named registers, compact sparse indices, or preserve a physical-hardware mapping. |
| Ordering | Emits measurements in their AST source order among gates. | Reorder operations, optimize circuits, or infer commutation. |
| Validation | Retains exporter rejection for unsupported AST statements and validates generated output through the existing reference parser and Qiskit importer checks. | Prove measurement semantics, simulate output distributions, submit jobs, or validate a provider. |
| Classical integration | Establishes an OpenQASM classical bit target only. | Add SynQ variables, values, conditionals, loops, or result handling. |

## Compatibility and exit criteria

Existing no-measurement source produces byte-identical bounded OpenQASM output:
no classical register is emitted unless a measurement exists. Existing direct
AST constructors remain valid. The parser and C ABI produce `SYNQ-P008` for
malformed measurement operands; exporter/parser smoke tests cover typed
measurement fields and exact output; and the generated fixture is accepted by
the independent OpenQASM 3 parser and Qiskit importer.

Completion establishes a **typed measurement source-export profile**, not a
quantum execution model, observed result API, general measurement syntax,
classical runtime, or hardware integration. Remote compiler-core evidence is
required before the increment is described as remotely validated.
