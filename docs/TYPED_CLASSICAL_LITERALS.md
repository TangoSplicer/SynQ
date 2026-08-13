# SynQ Typed Classical Literal Boundary

**Status:** Remotely validated recovery-profile implementation boundary.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s first typed quantum AST boundary gives the quantum side an inspectable
shape. The existing classical declaration form, `let name = value`, deliberately
retains its right-hand side as arbitrary source text. This increment implements
the smallest compatible step toward a hybrid AST: each parsed declaration keeps
its exact `value` while receiving a non-evaluating `ClassicalLiteralKind` hint.

> **Design rule:** classification must never reject source that the current
> recovery parser accepts, rewrite the stored value, evaluate code, or claim a
> complete classical type system. Unrecognised text is represented explicitly
> as source text rather than guessed.

## Initial vocabulary

| Kind | Exact recognition rule | Examples | Meaning and boundary |
| --- | --- | --- | --- |
| `Integer` | Optional leading `-`, then one or more decimal digits. | `0`, `42`, `-7` | A syntactic integer literal only; numeric range and arithmetic semantics are not introduced. |
| `Decimal` | Optional leading `-`, decimal digits with exactly one decimal point, and at least one digit. | `0.5`, `-2.0`, `.25` | A syntactic decimal literal only; precision, NaN, exponent forms, and arithmetic semantics remain future work. |
| `Boolean` | Exact lower-case `true` or `false`. | `true`, `false` | A syntactic boolean literal only; control-flow semantics remain future work. |
| `QuotedString` | Starts and ends with a double quote and has at least two characters. | `"synq"`, `"hello world"` | The raw source text is retained; escaping, interpolation, and Unicode normalisation are not interpreted. |
| `SourceText` | Any other accepted declaration right-hand side. | `theta + delta`, `https://example.invalid`, `model.v1` | Explicitly marks source that requires a later expression/type layer. |

## AST and parser contract

`DeclarationNode` now has a `literal_kind` field, defaulting to `SourceText`
for direct legacy construction. Its existing `name`, `value`, `line`, and
`toString()` output remain unchanged. The parser classifies only after the
existing declaration syntax validates the identifier and a non-empty
right-hand side.

| Input | Stored `value` | Expected classification |
| --- | --- | --- |
| `let shots = 1024` | `1024` | `Integer` |
| `let theta = -0.25` | `-0.25` | `Decimal` |
| `let enabled = true` | `true` | `Boolean` |
| `let label = "bell state"` | `"bell state"` | `QuotedString` |
| `let endpoint = https://example.invalid` | `https://example.invalid` | `SourceText` |

The classification applies only to declarations. It does not alter the existing
raw string arguments of `print`, `delay`, `ai`, quantum literal-angle syntax,
or the C ABI. The OpenQASM exporter continues rejecting declarations as
non-quantum source rather than attempting to lower classical values.

## Non-goals

This boundary does not parse expressions, coerce values, enforce numeric ranges,
support arithmetic, compile classical control flow, implement variables at
runtime, or establish a public stable AST ABI. It is deliberately smaller than
a type checker and is only a foundation for later expression, type, and Hybrid
IR work.

## Exit criteria

Parsed declarations now receive the documented classification; source
preservation and legacy direct construction remain tested; the bounded typed
quantum and OpenQASM path remains unchanged; and compiler, frontend, and
backend checks pass locally. Repository status describes this as **literal
classification**, not as a classical type system or evaluator. The same
compiler profile passed in [Compiler Core #11][1] for commit `4cc1b13`.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31720781396 "SynQ Compiler Core #11"
