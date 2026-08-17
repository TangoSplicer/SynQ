# Strict Hybrid IR OpenQASM 3 Export

**Status:** Remotely validated strict source-generation subset, including
Alpha-gated named-register operands and bounded identifier-`if` gate lowering,
in [Compiler Core #32069791226][3].
**Last reviewed:** 17 August 2026

## Purpose

`export_hybrid_openqasm3` provides a separate internal source-generation path
from typed Hybrid IR. It proves a narrow migration route away from direct AST
export without changing the existing AST exporter or public C ABI.

> **Design rule:** lower only typed structures with an exact mapping. Reject
> every Hybrid node whose SynQ meaning cannot be preserved in this bounded
> OpenQASM 3 subset.

## Accepted internal subset

The export path accepts declared typed qubit registers, supported typed gates,
unnamed measurements within each declaration’s range, and top-level Boolean
literal declarations. It emits declared OpenQASM qubit registers, deterministic
per-register classical measurement storage, and immutable generated Boolean
target storage. It lowers one literal `if` condition or one whole identifier
condition whose earlier source declaration is exactly `true` or `false`, with one
supported typed gate body. The direct AST exporter remains default-register-only.

Non-Boolean or non-literal classical declarations, declaration aliases, named
measurement-result declarations, missing explicit register declarations,
out-of-range operands, Boolean expressions, measurement-result conditions,
`while`, and `if` measurement bodies are rejected. The function returns a
diagnostic list and no program text on failure. See
[`IDENTIFIER_IF_LOWERING.md`](./IDENTIFIER_IF_LOWERING.md) for the exact
target-storage boundary.

## Explicit non-goals

This internal path does not change the public C ABI, OpenQASM import, named
measurement-result lowering, general control-flow lowering, runtime execution,
simulation, provider integration, or hardware submission.

## Focused validation

`synq_hybrid_openqasm3_exporter_smoke` verifies exact default-register,
named-register, literal-if, and identifier-if gate output. It also verifies
rejection for declaration aliases, Boolean expressions, missing declarations,
out-of-range operands, named measurement results, `while`, and `if` measurement
bodies. The separate CLI fixture is accepted by the OpenQASM 3 reference parser.
The local recovery profile and [Compiler Core #32069791226][3] both reported
**31/31** Linux CTest checks; Windows/MSVC and macOS/Clang each reported
**23/23** platform-neutral checks.

## CLI access

`synqc file.synq --emit-openqasm-hybrid [--out output.qasm]` first parses,
lowers, and resolves source before calling this exporter. It preserves strict
rejection behavior rather than falling back to the broader AST exporter. The
CLI smoke covers default-register Bell lowering, Alpha named-register lowering,
literal-if lowering, and bounded identifier-if gate lowering. [Compiler Core
#32069791226][3] passed the five-job matrix for this current CLI boundary.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31849244490 "SynQ Compiler Core #38"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31887104395 "SynQ Compiler Core #44"
[3]: https://github.com/TangoSplicer/SynQ/actions/runs/32069791226 "SynQ Compiler Core identifier-if platform matrix"
