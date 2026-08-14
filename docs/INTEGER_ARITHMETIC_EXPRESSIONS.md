# Bounded Integer Arithmetic Expressions

**Status:** Remotely validated Alpha-gated recovery-profile implementation in
[Compiler Core #31][1].
**Last reviewed:** 14 August 2026

## Purpose

This proposed recovery-profile increment promotes one deliberately bounded
integer arithmetic form from preserved opaque declaration source into a typed,
provenance-carrying internal expression tree. It is a static validation feature,
not an evaluator, runtime, or general mathematics language.

> **Design rule:** one operator, two atoms, and no value computation. An atom is
> an Integer literal or a whole identifier that resolves to an earlier Integer
> declaration. The resolver records provenance without calculating a result.

## Proposed Alpha grammar

After `#[experimental(feature = "integer-arithmetic-expressions")]`, a
declaration initializer may contain exactly one of these source forms:

| Source form | Typed tree | Resolver behavior |
| --- | --- | --- |
| `7`, `-2`, or `count` | Existing integer-literal or identifier classification. | A literal is statically Integer; an identifier must resolve to an earlier binding. |
| `count + 1` | `Add` with two integer atoms. | Validates both atoms in source order and records each identifier binding index. |
| `total - offset` | `Subtract` with two integer atoms. | Validates both earlier Integer references without subtraction. |
| `width * 4` | `Multiply` with two integer atoms. | Validates both operands without multiplication. |

The parser will reject multiple operators, parentheses, unary arithmetic other
than a signed integer literal, decimal operands, strings, Boolean operands,
function calls, comparisons, assignment, and arbitrary source-text leaves after
the Alpha feature is enabled. Without the opt-in, arithmetic-looking declaration
text remains existing `SourceText`/opaque source; it receives neither the typed
tree nor arithmetic resolver behavior. This preserves the pre-existing recovery
parser contract while keeping the experimental semantic surface explicit.

## Planned representation and diagnostics

`ClassicalIntegerArithmeticExpression` will store an explicit operator kind,
the original source text, a source span, and exactly two typed atom nodes for
parser-created arithmetic expressions. The existing `ClassicalExpression` will
own this optional tree only when its kind is `IntegerArithmeticExpression`; it
will carry static type `Integer` pending resolver validation.

The resolver will use sequential top-level scope. An unknown or forward operand
will use a new `SYNQ-R003` diagnostic, a non-Integer operand will use
`SYNQ-T003`, and a manually constructed invalid internal tree will use
`SYNQ-T004`. Malformed source uses `SYNQ-P011`.

## Focused validation

`synq_integer_arithmetic_expression_smoke` covers typed Add, Subtract, and
Multiply tree construction; Hybrid IR provenance; source-ordered prior-binding
indices; unannotated opaque-source compatibility; malformed opted-in syntax;
unknown and non-Integer identifier atoms; and invalid manually constructed
internal trees. The local recovery profile and [Compiler Core #31][1] both
reported **19/19** CTest checks.

## Explicit non-goals

This increment does not implement evaluation, constant folding, overflow
behavior, arbitrary expression parsing, recursive trees, precedence,
parentheses, division, modulo, exponentiation, comparisons, type coercion,
inference beyond existing literals and resolved identifiers, assignment,
functions, control-flow semantics, OpenQASM lowering, simulation, provider
integration, or hardware execution.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31847601825 "SynQ Compiler Core #31"
