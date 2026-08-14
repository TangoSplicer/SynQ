# Bounded Classical Expressions and Static Types

**Status:** Remotely validated internal recovery-profile implementation in
[Compiler Core #29][1].
**Last reviewed:** 14 August 2026

## Purpose

The bounded classical-expression layer converts parser-preserved declaration
right-hand-side source into a small internal classification with a static type
where that classification is already unambiguous. It creates no values, does
not evaluate code, and does not parse general expressions.

> **Design rule:** preserve source and provenance first; assign a static type
> only for existing literal forms or an already resolved identifier reference.
> All other accepted source remains explicitly opaque.

## Implemented representation

| Source category | Internal expression kind | Initial static type | Resolver behavior |
| --- | --- | --- | --- |
| `1024` | `IntegerLiteral` | `Integer` | Retained without evaluation. |
| `0.25` | `DecimalLiteral` | `Decimal` | Retained without evaluation. |
| `true` or `false` | `BooleanLiteral` | `Boolean` | Retained without evaluation. |
| `"label"` | `QuotedStringLiteral` | `String` | Retained without evaluation. |
| `ready` | `IdentifierReference` | `Unknown` before resolution | Resolves only to an earlier top-level declaration; inherits that declaration’s static type. |
| `count + 1` | `OpaqueSource` | `Unknown` | Preserved exactly; no sub-expression parsing, name lookup, arithmetic, or inference occurs. |

`ClassicalExpression` is stored internally on every `HybridDeclaration` with
its original source text and source span. `ResolvedHybridDeclaration` records
an optional earlier binding index plus the resulting static type. These are
internal C++ data structures, not a public ABI, serialized format, or language
compatibility commitment.

## Alpha condition-reference boundary

The existing Alpha `classical-control-flow` feature now accepts either a
literal Boolean condition or a whole identifier condition:

```synq
#[experimental(feature = "classical-control-flow")]
let ready = true
let inherited = ready
if inherited then quantum h q[0]
while ready do measure q[0]
```

The parser preserves the identifier condition without guessing its type. The
resolver requires an earlier binding and requires that binding’s propagated
static type to be `Boolean`. Unknown or forward condition references return
`SYNQ-R002`; non-Boolean references return `SYNQ-T001`. Neither result causes
the branch or loop to execute.

## Focused validation

`synq_classical_expression_smoke` covers literal classifications, opaque source
preservation, Boolean type propagation through a declaration alias, successful
resolved condition references, unknown condition rejection, and non-Boolean
condition rejection. The clean local recovery profile and [Compiler Core #29][1]
both reported **17/17** CTest checks, including this new smoke test.

## Explicit non-goals

This increment does not implement arithmetic, comparison, logical operators,
unary operators, parentheses, expression parsing, implicit conversion,
coercion, constraints, functions, assignments, mutability, variable values,
measurement results, general type inference, type annotations, generics,
blocks, `else`, nested scopes, evaluation, control-flow execution, OpenQASM
lowering, simulation, provider submission, or hardware execution.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31837377648 "SynQ Compiler Core #29"
