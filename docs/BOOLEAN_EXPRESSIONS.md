# Bounded Boolean Expressions

**Status:** Remotely validated Alpha-gated recovery-profile implementation in
[Compiler Core #30][1].
**Last reviewed:** 14 August 2026

## Purpose

The bounded Boolean-expression profile extends the existing Alpha
`classical-control-flow` condition slot without adding a general expression
language. It creates a typed, provenance-carrying internal tree for one explicit
operator form over Boolean literals or whole declaration identifiers.

> **Design rule:** retain a small grammar with no precedence ambiguity and no
> evaluation. Every identifier leaf must resolve to an earlier declaration of
> propagated static type `Boolean` before the internal control representation is
> considered resolved.

## Accepted Alpha grammar

After `#[experimental(feature = "classical-control-flow")]`, the condition
before `then` or `do` accepts exactly one of the following forms:

| Source form | Typed tree | Resolver behavior |
| --- | --- | --- |
| `true`, `false`, or `ready` | Boolean-literal or identifier leaf. | A literal needs no binding; an identifier must resolve to an earlier Boolean declaration. |
| `not ready` | `Not` with one literal/identifier leaf. | Validates its one leaf without computing a negated value. |
| `ready and enabled` | `And` with two literal/identifier leaves. | Validates both leaves in source order without computing a conjunction. |
| `ready or fallback` | `Or` with two literal/identifier leaves. | Validates both leaves in source order without computing a disjunction. |

The parser deliberately rejects parentheses, mixed/nested operators such as
`not ready and enabled`, repeated operators, arithmetic/comparison syntax,
function calls, and multi-token leaves with `SYNQ-P009`. This avoids inventing
precedence, associativity, implicit conversion, or general-expression behavior.

## Internal representation and diagnostics

`ClassicalBooleanExpression` stores its explicit kind, exact source text, source
span, literal flag where applicable, and zero, one, or two copied operands. It
is carried by the existing typed `ClassicalCondition`, copied through Hybrid IR,
and read by the name resolver without parser pointers, C ABI handles, or runtime
values.

Every identifier leaf follows the existing safe sequential-scope rule. An
unknown or forward identifier returns `SYNQ-R002`; a resolved Integer, Decimal,
String, or Unknown type returns `SYNQ-T001`. `SYNQ-T002` protects the internal
resolver from an invalid manually constructed Boolean-tree shape. Parser-created
trees always conform to the bounded grammar.

## Focused validation

`synq_boolean_expression_smoke` covers typed `not`, `and`, and `or` tree
construction; Hybrid IR provenance; ordered Boolean binding indices; unknown
and non-Boolean leaves; unsupported operator combinations; and the existing
Alpha feature gate. The clean local recovery profile and [Compiler Core #30][1]
both reported **18/18** CTest checks, including this new smoke test.

## Explicit non-goals

This increment does not implement value evaluation, short-circuit behavior,
general recursive expressions, operator precedence, parentheses, comparisons,
arithmetic, assignment, type coercion, type inference beyond existing literals
and bound identifiers, measurement-result values, blocks, `else`, nested control
flow, branch/loop execution, OpenQASM control lowering, simulation, provider
submission, or hardware execution.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31842571512 "SynQ Compiler Core #30"
