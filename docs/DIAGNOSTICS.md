# SynQ Structured Diagnostics

**Status:** Remotely validated recovery-profile implementation boundary.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s recovery parser currently recognizes a deliberately small language
profile. Until this increment, parser failures were emitted directly to
standard error and represented only by a `nullptr` return. That makes failures
hard for the C ABI, future editors, and future language bindings to consume
reliably.

This increment implements the smallest durable replacement: a typed parser
result containing an optional parsed program and a collection of structured
diagnostics. It does **not** define a complete lexer, a type checker, module
system, or package manager. A later internal Hybrid IR reuses the same
diagnostic struct for its bounded lowering rejection.

> **Design rule:** every recovery-parser failure represented by this increment
> receives a stable code, a severity, a half-open source span, a message, and
> optional remediation. The legacy `parseFile()` API remains a compatibility
> wrapper until callers migrate.

## Public model

| Type | Required fields | Contract |
| --- | --- | --- |
| `SourceSpan` | `line`, `column_start`, `column_end` | Locations are one-based; `column_start` is inclusive and `column_end` is exclusive. A whole-line parser error spans the trimmed statement text. |
| `DiagnosticSeverity` | `Error`, `Warning` | This increment produces parser errors. Warnings are reserved for later alpha/beta feature notices. |
| `Diagnostic` | `code`, `severity`, `span`, `message`, `help` | Codes use the stable recovery-parser prefix `SYNQ-P`. `help` is optional and must be an actionable plain-language suggestion when present. |
| `ParseResult` | `program`, `diagnostics` | `program` is owned by the result. A result is successful only when it owns a non-null `ProgramNode` and contains no error diagnostics. |

The owned program changes the internal parser API without changing the existing
raw-pointer compatibility method. `parseFile()` may return the owned program to
legacy callers only when `ParseResult::ok()` is true; otherwise it returns
`nullptr` after rendering the structured diagnostics for the command line.

## Initial parser-code registry

The first registry is intentionally small. These codes are implemented by the
recovery-profile parser and covered by a focused smoke test.

| Code | Failure condition | Default remediation |
| --- | --- | --- |
| `SYNQ-P001` | Source file cannot be opened. | Verify the path and read permission. |
| `SYNQ-P002` | A `let` declaration has no valid identifier or value. | Use `let <identifier> = <value>`. |
| `SYNQ-P003` | The line does not start with a supported recovery-profile instruction or a required argument is absent. | Use `let`, `print`, `delay`, `quantum`, `measure`, or `ai` with the documented argument form. |
| `SYNQ-P004` | `delay` does not use a non-negative integer number of milliseconds. | Use a non-negative whole number, such as `delay 0`. |
| `SYNQ-P005` | Quantum operands or parameter syntax do not match the bounded grammar. | Use explicit `q[index]` operands and the documented literal-angle form. |
| `SYNQ-P006` | An experimental annotation is malformed or names an unknown feature. | Use a registered exact annotation such as `#[experimental(feature = "parameterized-quantum-gates")]`. |
| `SYNQ-P007` | A gated construct is used without its required alpha opt-in. | Add the documented file-scoped feature annotation or use an ungated construct. |
| `SYNQ-P008` | A measurement does not use exactly one explicit non-negative qubit operand. | Use `measure q[index]`, for example `measure q[0]`. |
| `SYNQ-P009` | Alpha-gated classical control flow does not use its exact literal-boolean `if ... then ...` or `while ... do ...` form. | Use `if true then quantum h q[0]` or `while false do measure q[0]` after enabling the feature. |
| `SYNQ-P010` | An Alpha-gated classical-control-flow body is not exactly one bounded quantum gate or measurement. | Use one `quantum` gate statement or one `measure q[index]` body. |

Code values are specific to parser failures. The recovery profile also has a
small `SYNQ-S001`–`SYNQ-S004` namespace for typed known-gate shape validation
and bounded duplicate-declaration validation; those semantic errors use the
same span, severity, message, help, legacy rendering, and C ABI formatting
path. Backend export diagnostics and C ABI status values keep their own
namespaces because they represent different layers and different recovery
actions.

| Code | Layer | Failure condition | Default remediation |
| --- | --- | --- | --- |
| `SYNQ-H001` | Internal Hybrid IR lowering | A successful recovery parse contains a retained legacy AST instruction that the minimal Hybrid IR does not model. | Use only current typed declarations, quantum gates, and measurements until the internal IR expands. |

`SYNQ-H001` is not emitted by the parser and is not propagated through the C
ABI. It preserves source provenance for an internal post-parse conversion
boundary; it does not define a general HIR diagnostics protocol. Its focused
Hybrid IR smoke coverage passed remotely in [Compiler Core #23][2].

| Code | Layer | Failure condition | Default remediation |
| --- | --- | --- |
| `SYNQ-R001` | Internal name resolution | A declaration initializer is a whole identifier but has no earlier top-level declaration in the same Hybrid IR program. | Declare the name on an earlier top-level line or use a supported literal. |

`SYNQ-R001` is not emitted by the parser and is not propagated through the C
ABI. It identifies only the first sequential top-level name-resolution boundary;
it does not define lexical scopes, imports, expression resolution, or a general
symbol-table protocol. Its focused smoke coverage passed remotely in [Compiler
Core #26][3].

## Compatibility boundaries

| Existing caller | Increment behavior | What is deliberately unchanged |
| --- | --- | --- |
| C++ recovery-profile caller | `parseFile()` continues returning `ASTNode*` or `nullptr`; it renders structured diagnostics to standard error for the legacy path. | The current AST ownership convention and recovery grammar are not redesigned in this increment. |
| C ABI caller | `synq_parse_file()` consumes the structured parse result and returns a formatted diagnostic string on failure. | The public C ABI signatures and ownership rules remain unchanged. |
| OpenQASM exporter | Receives the same successful `ProgramNode` shape. | It does not perform parser recovery, type checking, or execution. |
| Future editor/binding | May consume typed code, severity, span, message, and help through a later API. | No LSP, JSON diagnostic transport, Rust/Mercury/Lisp/Clojure binding, or IDE protocol is claimed. |

## Formatting and safety rules

The canonical text rendering is:

```text
<file>:<line>:<column>: error[SYNQ-P005]: malformed quantum operands
  help: use explicit operands such as q[0] or q[0], q[1]
```

Text rendering is a convenience layer; callers should not parse it. Tests must
assert structured fields directly and use rendering tests only to protect the
human-facing message format. The parser stops at the first fatal error in this
recovery profile, preserving its existing all-or-nothing behavior. Multi-error
recovery, error suppression, localisation, source snippets, and suggestion
ranking are later design work.

The parser must never encode source text into a diagnostic in a way that turns
it into a command, file path, network request, or executable action. Diagnostics
are inert strings describing a local source failure.

## Exit criteria for this increment

The compiler now contains the typed model; every initial code has a focused
fixture; the legacy API behavior remains covered; and the C ABI exposes a
formatted `SYNQ-P001` missing-file diagnostic. The full local validation run
passes **8/8** compiler checks alongside the existing frontend and backend
checks. The same compiler profile passed in [Compiler Core #9][1] for commit
`5be6747`. The project status record retains the non-claims above.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31719597241 "SynQ Compiler Core #9"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31803349469 "SynQ Compiler Core #23"
[3]: https://github.com/TangoSplicer/SynQ/actions/runs/31803737501 "SynQ Compiler Core #26"
