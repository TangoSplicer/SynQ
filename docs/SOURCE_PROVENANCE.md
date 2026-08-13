# SynQ AST Source Provenance

**Status:** Locally verified recovery-profile language-core boundary; remote CI
evidence for this increment is pending publication.
**Last reviewed:** 13 August 2026

## Purpose

SynQ already records `SourceSpan` data on structured diagnostics. Its typed AST
nodes previously carried only a line number, which made future Hybrid IR
provenance, editor integrations, and backend diagnostics depend on reparsing
source or reconstructing locations. This increment gives parsed statement nodes
the same source-span vocabulary without changing the bounded grammar or C ABI.

> **Design rule:** parser-produced AST nodes own a copy of their statement span.
> A legacy direct constructor remains valid and represents an unknown column
> range rather than inventing precision that was never supplied.

## Provenance contract

| Node | New field | Parsed-node invariant | Direct legacy-construction behavior |
| --- | --- | --- | --- |
| `InstructionNode` | `SourceSpan span` | `line == span.line`; the span covers the trimmed, comment-free statement. | `span = {line, 0, 0}`. |
| `DeclarationNode` | `SourceSpan span` | `line == span.line`; the span covers `let <name> = <value>` after existing comment/semicolon handling. | `span = {line, 0, 0}`. |
| `QuantumGateNode` | `SourceSpan span` | `line == span.line`; the span covers the accepted quantum statement. | `span = {line, 0, 0}`. |
| `ProgramNode` | No aggregate span in this increment. | Child nodes retain their individual spans. | Unchanged. |

`SourceSpan` keeps its existing semantics: lines and known columns are one-based,
`column_start` is inclusive, and `column_end` is exclusive. A zero
`column_start` and `column_end` denotes a location known only by line, which is
the compatibility state for direct constructors. A zero line denotes an unknown
location and is not generated for successfully parsed statements.

## Parser and compatibility boundary

The recovery parser already creates a statement span for diagnostics. It now
passes that exact value to each newly parsed AST node. This prevents a source
statement and an error about that statement from silently disagreeing on
location. The existing public node constructors keep their current arguments;
an optional final span parameter defaults from the supplied line number.

| Concern | This increment does | This increment does not do |
| --- | --- | --- |
| Parser | Copies its existing per-statement span into parsed nodes. | Add a lexer, byte offsets, file identifiers, multi-line spans, or recovery across errors. |
| Diagnostics | Shares a value model with AST provenance. | Link a diagnostic to a node identity, expose a new public C ABI record, or add JSON/LSP transport. |
| Exporter | Continues using existing typed fields and line diagnostics. | Change OpenQASM output, source mapping syntax, or hardware behavior. |
| Future HIR | Can receive source provenance from typed AST nodes. | Implement an HIR, semantic type system, editor service, or public stable AST ABI. |

## Exit criteria

Parsed declaration, instruction, and typed quantum nodes now carry the
documented span; legacy constructors retain their previous call shape and
receive an unknown-column span; parser fixtures assert line/column invariants;
and compiler, frontend, backend, exporter, and C ABI checks pass locally.
Repository material describes this as **AST source provenance**, not an IDE,
source-map, or Hybrid IR feature. Remote compiler-core evidence is required
before the increment is described as remotely validated.
