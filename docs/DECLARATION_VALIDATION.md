# SynQ Bounded Declaration Validation

**Status:** Remotely validated recovery-profile classical semantic boundary.
**Last reviewed:** 13 August 2026

## Purpose

SynQ now preserves typed classical literal hints and source spans for top-level
`let` declarations. Without a bounded declaration rule, a source file could
silently repeat a name, leaving the recovery profile with no explicit,
deterministic binding policy. This increment implements the smallest safe
classical semantic boundary: unique top-level declaration names per parsed
source file.

> **Design rule:** reject only a repeated valid `let` identifier in the same
> parse invocation. Do not infer expression values, import names, function
> scopes, mutation, shadowing, module behavior, or cross-file identity.

## Rule and diagnostic

The parser keeps a case-sensitive map from each accepted declaration identifier
to the first declaration’s `SourceSpan`. Before a later declaration is added to
the AST, the parser checks whether the identifier has already been recorded.

| Condition | Result | Structured diagnostic |
| --- | --- | --- |
| First `let theta = 0.5` in a source file | Accepted and recorded. | None. |
| Later `let theta = 1.0` in the same parse invocation | Rejected before it enters the AST. | `SYNQ-S004` at the later declaration’s span; message names the original declaration line. |
| `let Theta = 1.0` after `let theta = 0.5` | Accepted. | SynQ identifiers are currently case-sensitive. |
| Same name in a separate `Parser::parseFileWithDiagnostics()` invocation | Accepted independently. | No cross-file symbol table exists. |

The default help text asks the author to rename the later binding or reuse the
original name according to future language semantics. Because the recovery
profile has no assignment syntax, it does not suggest a mutation form.

## Compatibility boundary

The check now runs after existing identifier/value syntax validation and before
`DeclarationNode` construction. Invalid declaration syntax keeps `SYNQ-P002`.
The duplicate-binding rule uses the existing `SYNQ-S` semantic namespace and
the typed diagnostic/C ABI formatting path. Successful single declarations,
typed literal classification, source provenance, quantum statements, OpenQASM
export, and direct AST constructors remain unchanged.

This is **not** a general symbol table. It intentionally does not implement
scopes, imports, functions, blocks, closures, module resolution, namespaces,
read-before-write checks, use-site resolution, variable evaluation, or type
checking. Those require future grammar and typed semantic layers.

## Exit criteria

A duplicate top-level declaration now produces `SYNQ-S004` with the later
statement span and a first-line reference; separate parse invocations remain
independent; parser diagnostics and C ABI consumers cover the code; and
compiler, frontend, backend, exporter, and external OpenQASM checks pass
locally. Repository status calls this **bounded declaration validation**, not a
symbol table or name-resolution system. The same compiler profile passed in
[Compiler Core #14][1] for commit `fd5345c`.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31722554030 "SynQ Compiler Core #14"
