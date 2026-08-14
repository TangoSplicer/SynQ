# Bounded Scoped Name Resolution

**Status:** The earlier declaration-only resolver passed remotely in [Compiler
Core #26][1]; the Boolean condition-reference extension is locally validated
and awaits remote compiler-core evidence.
**Last reviewed:** 14 August 2026

## Purpose

This first name-resolution pass resolves two deliberately narrow reference
forms: a declaration whose complete right-hand side is an identifier, and an
Alpha control condition that is a whole identifier. It traverses the minimal
internal Hybrid IR in source order and may connect either form only to an
**earlier top-level declaration** in the same parsed program.

> **Design rule:** a binding edge must be explicit, deterministic, and backed
> by source provenance. The pass must reject an unknown or forward identifier
> rather than guess a scope, evaluate an expression, or defer an untracked
> reference.

## Implemented contract

| Input pattern | Resolution behavior | Boundary |
| --- | --- | --- |
| `let shots = 1024` | Retains the integer literal and `Integer` static type; no binding edge is created. | No runtime value, mutability, or constant evaluation is inferred. |
| `let selected = shots` after `let shots = 1024` | `ResolvedHybridDeclaration::initializer_binding_index` stores the earlier declaration’s Hybrid IR node index and inherits its static type. | This is a top-level sequential scope only; the index is internal and has no public ABI or serialization contract. |
| `let expression = one + two` | Retains `SourceText` as opaque and creates no binding edge. | No expression parser, partial sub-expression resolution, overload lookup, or symbol guessing occurs. |
| `let selected = missing` or a forward declaration | Produces `SYNQ-R001` at the referring declaration span and returns no partial resolved program. | No imports, globals, implicit variables, deferred linking, or forward declarations are supported. |
| `if ready then quantum h q[0]` | Connects the condition to an earlier Boolean declaration and stores its binding index on `ResolvedHybridControlFlow`. | No value read, truthiness, coercion, expression condition, or branch execution occurs. |
| `if missing then quantum h q[0]` | Produces `SYNQ-R002`; a resolved Integer, Decimal, String, or Unknown binding produces `SYNQ-T001`. | No implicit Boolean conversion or runtime type check is attempted. |
| Quantum gate or measurement node | Preserves the typed Hybrid IR node unchanged and in source order. | Gate operands are integer indices, not resolved qubit bindings or allocated resources. |

## Data and ownership boundary

`resolve_hybrid_names(const HybridProgram&)` produces a separate
`ResolvedHybridProgram` variant sequence. The source Hybrid IR remains
unmodified, and no AST pointer, C ABI handle, parser allocation, or external
resource is retained. A successful result has a complete resolved program; an
error result has only diagnostics.

The resolver uses the parser’s existing duplicate-top-level-declaration rule as
its input invariant. It does not introduce a second duplicate-binding policy.

## Focused validation

`synq_name_resolution_smoke` retains coverage for earlier declaration references
and opaque source. The separate `synq_classical_expression_smoke` covers Boolean
type propagation and resolved control conditions. The latter has local 17-test
evidence only until this expression/type increment is published and run in CI.

## Explicit non-goals

This pass does not provide nested lexical scopes, blocks, functions, modules,
imports, qualified names, shadowing, assignment, mutability, expression parsing,
general type checking, generics, closures, qubit declarations or liveness,
measurement results, general condition expressions, runtime execution, OpenQASM
lowering, a public API, or a stable resolved-IR serialization format.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31803737501 "SynQ Compiler Core #26"
