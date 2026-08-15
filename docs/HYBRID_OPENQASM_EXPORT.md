# Strict Hybrid IR OpenQASM 3 Export

**Status:** Remotely validated internal source-generation extension in [Compiler
Core #38][1]. A locally validated `synqc --emit-openqasm-hybrid` command now
exposes the same strict contract to recovery-profile users; its CLI evidence is
pending publication and compiler-core CI.
**Last reviewed:** 14 August 2026

## Purpose

`export_hybrid_openqasm3` provides a separate internal source-generation path
from typed Hybrid IR. It proves a narrow migration route away from direct AST
export without changing the existing AST exporter or public C ABI.

> **Design rule:** lower only typed structures with an exact mapping. Reject
> every Hybrid node whose SynQ meaning cannot be preserved in this bounded
> OpenQASM 3 subset.

## Accepted internal subset

The export path requires exactly one `HybridQubitDeclaration` named `q`, then
accepts supported typed gates and unnamed measurements within that declaration's
range. It emits `qubit[n] q;`, emits `bit[n] c;` only if an unnamed measurement
exists, and reuses the existing supported gate spellings and literal-angle rules.

Classical declarations, named-register declarations, named measurement-result
declarations, typed control nodes, missing explicit `qubit q[n]` declarations,
and out-of-range operands are rejected. The function returns a diagnostic list
and no program text on failure.

## Explicit non-goals

This internal path does not change the public C ABI, OpenQASM import, named
register operands, named measurement-result lowering, control-flow lowering,
runtime execution, simulation, provider integration, or hardware submission.

## Focused validation

`synq_hybrid_openqasm3_exporter_smoke` verifies exact output for an explicit
typed `qubit q[n]` declaration, a supported gate, and an unnamed measurement.
It also verifies rejection for missing declarations, out-of-range operands,
named measurement-result declarations, and unlowered typed control nodes. The
local recovery profile and [Compiler Core #38][1] both reported **22/22** CTest
checks.

## CLI access

`synqc file.synq --emit-openqasm-hybrid [--out output.qasm]` first parses,
lowers, and resolves source before calling this exporter. It preserves the
existing explicit rejection behavior rather than falling back to the broader AST
exporter. The CLI smoke covers an explicit `qubit q[2]` Bell-pair fixture and
asserts the declared register size and `h`/`cx` lowering. This additional CLI
access remains local evidence pending publication and CI.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31849244490 "SynQ Compiler Core #38"
