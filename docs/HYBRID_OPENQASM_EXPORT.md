# Strict Hybrid IR OpenQASM 3 Export

**Status:** Remotely validated strict source-generation subset, including
Alpha-gated named-register operands and literal-if gate lowering, in [Compiler
Core #48][3].
**Last reviewed:** 16 August 2026

## Purpose

`export_hybrid_openqasm3` provides a separate internal source-generation path
from typed Hybrid IR. It proves a narrow migration route away from direct AST
export without changing the existing AST exporter or public C ABI.

> **Design rule:** lower only typed structures with an exact mapping. Reject
> every Hybrid node whose SynQ meaning cannot be preserved in this bounded
> OpenQASM 3 subset.

## Accepted internal subset

The export path accepts declared typed qubit registers, supported typed gates,
and unnamed measurements within each declaration’s range. It emits declared
OpenQASM qubit registers and deterministic per-register classical measurement
storage. It also lowers exactly one literal `if` condition with one supported
typed gate body. The direct AST exporter remains default-register-only.

Classical declarations, named measurement-result declarations, missing explicit
register declarations, out-of-range operands, identifier/expression conditions,
`while`, and literal-if measurement bodies are rejected. The function returns a
diagnostic list and no program text on failure.

## Explicit non-goals

This internal path does not change the public C ABI, OpenQASM import, named
measurement-result lowering, general control-flow lowering, runtime execution,
simulation, provider integration, or hardware submission.

## Focused validation

`synq_hybrid_openqasm3_exporter_smoke` verifies exact default-register,
named-register, and literal-if gate output. It also verifies rejection for
missing declarations, out-of-range operands, named measurement results,
identifier conditions, `while`, and literal-if measurement bodies. The local
recovery profile and [Compiler Core #48][3] both reported **27/27** CTest checks.

## CLI access

`synqc file.synq --emit-openqasm-hybrid [--out output.qasm]` first parses,
lowers, and resolves source before calling this exporter. It preserves strict
rejection behavior rather than falling back to the broader AST exporter. The
CLI smoke covers default-register Bell lowering, Alpha named-register lowering,
and Alpha literal-if gate lowering. [Compiler Core #48][3] passed **27/27**
checks for this current CLI boundary.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31849244490 "SynQ Compiler Core #38"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31887104395 "SynQ Compiler Core #44"
[3]: https://github.com/TangoSplicer/SynQ/actions/runs/31952214849 "SynQ Compiler Core #48"
