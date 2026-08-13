# Minimal Hybrid IR

**Status:** Locally validated internal recovery-profile implementation; remote
compiler-core evidence is pending publication of this increment.
**Last reviewed:** 13 August 2026

## Purpose

The minimal Hybrid IR is the first internal program representation that can hold
the currently typed **classical declaration**, **quantum gate**, and
**measurement** nodes in one ordered sequence. It sits after successful parsing
and before future name resolution, type checking, control-flow modeling, or
backend lowering. It is deliberately not a public API, C ABI type, serialized
artifact, or executable representation.

> **Design rule:** the first Hybrid IR preserves typed parser data and source
> provenance exactly; unsupported nodes are rejected with a structured
> diagnostic rather than dropped or assigned invented semantics.

## Implemented model

| Hybrid node | Preserved fields | Current boundary |
| --- | --- | --- |
| `HybridDeclaration` | Binding name, exact right-hand-side source text, `ClassicalLiteralKind`, and `SourceSpan`. | It does not evaluate an expression, assign a static type, model mutability, or create a runtime value. |
| `HybridQuantumGate` | `QuantumGateKind`, original gate spelling, optional literal-angle text, ordered qubit indices, and `SourceSpan`. | It does not allocate or own qubits, evaluate angles, define gate semantics, or perform backend lowering. |
| `HybridMeasurement` | Qubit index and `SourceSpan`. | It does not allocate a result value, expose a measured bit, define result lifetime, or enable measurement-dependent control flow. |
| `HybridProgram` | One ordered `std::variant` sequence of the three supported node types. | It is internal C++ data only; it has no public ABI, stable serialization format, or compatibility commitment. |

`lower_to_hybrid_ir(const ProgramNode&)` returns `HybridLoweringResult`. A
successful result owns a complete `HybridProgram`; any error result has no
partial program. The current converter accepts only the existing typed recovery
nodes. A legacy `InstructionNode`, such as accepted `print`, `delay`, or `ai`
recovery syntax, returns `SYNQ-H001` with the original source span.

## Provenance and safety boundary

The conversion copies `SourceSpan` values from typed AST nodes. It neither
retains AST pointers nor transfers AST ownership, so a later internal AST change
does not make HIR clients depend on parser allocation layout. It also preserves
literal-angle source text without evaluation, keeping the alpha-gated parser
boundary separate from future expression semantics.

The conversion does not parse new source, consult the C ABI, evaluate a
declaration, invoke an AI service, create threads, perform I/O, simulate a
circuit, submit work to a provider, or access hardware.

## Focused validation

`synq_hybrid_ir_smoke` covers an ordered declaration/gate/measurement source,
an alpha-gated `rx(pi/2) q[1]` node, all preserved provenance fields, and
rejection of a parser-accepted legacy instruction. The clean local recovery
profile reported **14/14** CTest checks passing, including the new smoke test.

## Explicit non-goals

This increment does not implement scoped names, references between declarations,
SSA values, expression typing, qubit allocation or liveness, result values,
classical assignment, `if` or `while`, blocks, functions, modules, imports,
optimization, serialization, code generation, OpenQASM lowering from HIR,
execution, simulation, or hardware integration.
