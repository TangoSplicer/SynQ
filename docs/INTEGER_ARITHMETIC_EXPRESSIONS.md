# Bounded Integer Arithmetic Expressions

**Status:** Design and implementation in progress; no remote validation claim is
made in this record.
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
`SYNQ-T004`. Malformed source will use `SYNQ-P011`. These codes are proposed
until the implementation and documentation registry are updated together.

## Explicit non-goals

This increment does not implement evaluation, constant folding, overflow
behavior, arbitrary expression parsing, recursive trees, precedence,
parentheses, division, modulo, exponentiation, comparisons, type coercion,
inference beyond existing literals and resolved identifiers, assignment,
functions, control-flow semantics, OpenQASM lowering, simulation, provider
integration, or hardware execution.
