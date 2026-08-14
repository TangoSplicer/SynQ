# Bounded Scoped Name Resolution

**Status:** Locally validated internal recovery-profile implementation; remote
compiler-core evidence is pending publication of this increment.
**Last reviewed:** 14 August 2026

## Purpose

This first name-resolution pass resolves one deliberately narrow reference form:
a declaration whose complete right-hand side is an identifier. It traverses the
minimal internal Hybrid IR in source order and may connect that initializer only
to an **earlier top-level declaration** in the same parsed program.

> **Design rule:** a binding edge must be explicit, deterministic, and backed
> by source provenance. The pass must reject an unknown or forward identifier
> rather than guess a scope, evaluate an expression, or defer an untracked
> reference.

## Implemented contract

| Input pattern | Resolution behavior | Boundary |
| --- | --- | --- |
| `let shots = 1024` | Retains the integer literal hint; no binding edge is created. | No static type, runtime value, mutability, or constant evaluation is inferred. |
| `let selected = shots` after `let shots = 1024` | `ResolvedHybridDeclaration::initializer_binding_index` stores the earlier declaration’s Hybrid IR node index. | This is a top-level sequential scope only; the index is internal and has no public ABI or serialization contract. |
| `let expression = one + two` | Retains `SourceText` as opaque and creates no binding edge. | No expression parser, partial sub-expression resolution, overload lookup, or symbol guessing occurs. |
| `let selected = missing` or a forward declaration | Produces `SYNQ-R001` at the referring declaration span and returns no partial resolved program. | No imports, globals, implicit variables, deferred linking, or forward declarations are supported. |
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

`synq_name_resolution_smoke` covers a valid earlier declaration reference, a
literal with no invented binding edge, opaque expression-like `SourceText`, an
unknown reference, a forward reference, source-order preservation, and source
span preservation. The clean local recovery profile reported **15/15** CTest
checks passing, including the new smoke test.

## Explicit non-goals

This pass does not provide nested lexical scopes, blocks, functions, modules,
imports, qualified names, shadowing, assignment, mutability, expression parsing,
type checking, generics, closures, qubit declarations or liveness, measurement
results, `if`, `while`, runtime execution, OpenQASM lowering, a public API, or a
stable resolved-IR serialization format.
