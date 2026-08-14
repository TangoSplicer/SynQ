# Bounded Qubit Declarations

**Status:** Remotely validated Alpha-gated recovery-profile implementation in
[Compiler Core #35][1].
**Last reviewed:** 14 August 2026

## Purpose

This recovery-profile increment introduces explicit, typed source metadata for
one named positive-size qubit register. It is a declaration and provenance
feature, not a runtime allocator, resource manager, or quantum execution model.

> **Design rule:** a qubit declaration records one identifier and one positive
> source-level size. It does not allocate qubits, infer a default register, or
> make an operation executable.

## Alpha grammar

After `#[experimental(feature = "qubit-declarations")]`, the parser accepts
exactly:

```synq
qubit register[3]
```

The identifier follows the recovery parser's existing identifier rule, and the
size is a positive decimal whole number. `qubit q[0]`, negative sizes, decimal
sizes, missing brackets, extra brackets, and invalid identifiers are rejected
with `SYNQ-P012`. The declaration shares the existing top-level declaration
namespace, so duplicate names use `SYNQ-S004`.

## Internal representation

`QubitDeclarationNode` preserves the name, positive source size, line, and
one-based source span. `HybridQubitDeclaration` copies that metadata through
the internal Hybrid IR, and the current name-resolution pass preserves it.

This first increment deliberately does **not** validate `quantum` or `measure`
operand indices against any declared register. Existing indexed operands remain
syntactically handled as `q[index]`; operation-to-register binding and range
checking are a following increment.

## Focused validation

`synq_qubit_declaration_smoke` covers accepted typed declaration construction,
source-span and Hybrid IR provenance, resolver preservation, Alpha gating,
positive-size/identifier rejection, shared top-level uniqueness, and the
deliberate absence of gate or measurement operand-bound validation. The local
recovery profile and [Compiler Core #35][1] both reported **20/20** CTest
checks.

## Explicit non-goals

This increment does not provide runtime allocation, qubit ownership, aliasing,
liveness, deallocation, borrow checking, named register operands, gate or
measurement bound checking, OpenQASM declaration lowering, simulation,
provider integration, hardware submission, or execution semantics.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31848161711 "SynQ Compiler Core #35"
