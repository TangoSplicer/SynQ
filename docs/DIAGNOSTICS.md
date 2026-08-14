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
| `SYNQ-P008` | A measurement does not use one explicit non-negative qubit operand with an optional valid result identifier. | Use `measure q[index]` or `measure q[index] as <identifier>`, for example `measure q[0] as observed`. |
| `SYNQ-P009` | Alpha-gated classical control flow does not use its exact bounded Boolean-literal/identifier, `not <atom>`, or `<atom> and/or <atom>` `if ... then ...` or `while ... do ...` form. | Use `if true then quantum h q[0]`, `if not ready then quantum h q[0]`, or `if ready and enabled then quantum h q[0]` after enabling the feature. |
| `SYNQ-P010` | An Alpha-gated classical-control-flow body is not exactly one bounded quantum gate or measurement. | Use one `quantum` gate statement or one `measure q[index]` body. |
| `SYNQ-P011` | An opted-in Integer arithmetic declaration does not use exactly two Integer literal/identifier atoms and one `+`, `-`, or `*` token. | Use `let total = count + 1` after enabling `integer-arithmetic-expressions`. |
| `SYNQ-P012` | An Alpha-gated qubit declaration does not use exactly one identifier and a positive decimal whole-number size. | Use `qubit q[2]` after enabling `qubit-declarations`. |

`SYNQ-P009` and `SYNQ-P010` are parser diagnostics for the Alpha
classical-control-flow profile. The Boolean-operator extension, including its
`SYNQ-P009` malformed-form coverage, passed remotely in [Compiler Core #30][5].

`SYNQ-P011` is restricted to source parsed after the
`integer-arithmetic-expressions` Alpha opt-in. Without that opt-in, existing
arithmetic-looking declaration text remains opaque source. Its focused smoke
coverage passed remotely with **19/19** CTest checks in [Compiler Core #31][6].

`SYNQ-P012` is restricted to source parsed after the `qubit-declarations` Alpha
opt-in. It has local and remote **20/20** focused smoke evidence in [Compiler
Core #35][7]. The code validates declaration shape only; it does not yet validate
`quantum` or `measure` operand indices against declared register sizes.

The `SYNQ-P008` measurement parser boundary also covers the local named-result
extension. [Compiler Core #37][9] validated the optional result-identifier form,
while preserving the existing unnamed measurement form. It does not establish a
runtime measurement value or named-result OpenQASM lowering.

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

| Code | Layer | Failure condition | Default remediation |
| --- | --- | --- | --- |
| `SYNQ-R002` | Internal name resolution | An Alpha classical-control condition identifier has no earlier top-level declaration. | Declare the name on an earlier line with static type `Boolean`, or use `true`/`false`. |
| `SYNQ-T001` | Internal static-type validation | An Alpha classical-control condition resolves to a declaration whose propagated static type is not `Boolean`. | Use a Boolean declaration/reference or a `true`/`false` literal. |
| `SYNQ-T002` | Internal Boolean-tree validation | A manually constructed internal Boolean expression has an unsupported operator or wrong operand count. | Use the parser-produced bounded tree shape: leaf, `not <atom>`, or `<atom> and/or <atom>`. |
| `SYNQ-R003` | Internal name resolution | An opted-in Integer arithmetic identifier atom has no earlier top-level declaration. | Declare the name on an earlier line with static type `Integer`, or use an Integer literal. |
| `SYNQ-T003` | Internal static-type validation | An opted-in Integer arithmetic identifier atom resolves to a non-Integer declaration. | Use an Integer declaration/reference or an Integer literal atom. |
| `SYNQ-T004` | Internal arithmetic-tree validation | A manually constructed internal Integer arithmetic expression has an unsupported operator or wrong operand count. | Use the parser-produced one-operator tree with two Integer literal/identifier atoms. |
| `SYNQ-Q001` | Internal default-register validation | `q[index]` occurs before an explicit later `qubit q[n]` declaration. | Declare the default register before operations using `q[index]`. |
| `SYNQ-Q002` | Internal default-register validation | A `q[index]` operand is not smaller than the explicit `qubit q[n]` size. | Use an index in the declared `q[0]` through `q[n-1]` range. |

`SYNQ-R002`, `SYNQ-T001`, and `SYNQ-T002` are internal resolver/type diagnostics.
They are not parser diagnostics and are not propagated through the C ABI. They
cover only the bounded Boolean-condition profile; they do not establish a
general expression or type system. Their focused Boolean-expression smoke
coverage passed remotely in [Compiler Core #30][5].

`SYNQ-R003`, `SYNQ-T003`, and `SYNQ-T004` are internal resolver/type diagnostics
for the bounded opted-in arithmetic profile. They are not parser diagnostics and
are not propagated through the C ABI. Their focused smoke coverage is local
and remote **19/19** evidence in [Compiler Core #31][6]; they do not establish
arithmetic evaluation or a general expression/type system.

`SYNQ-Q001` and `SYNQ-Q002` are internal resolver diagnostics for the local
default-register reference-validation extension. They apply only when a typed
`qubit q[n]` declaration exists anywhere in the Hybrid IR program; a program
without such a declaration retains prior indexed-operand behavior. They are not
parser diagnostics and are not propagated through the C ABI. Their focused smoke
coverage passed locally and remotely with **20/20** CTest checks in [Compiler
Core #36][8].

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
[4]: https://github.com/TangoSplicer/SynQ/actions/runs/31804184422 "SynQ Compiler Core #27"
[5]: https://github.com/TangoSplicer/SynQ/actions/runs/31842571512 "SynQ Compiler Core #30"
[6]: https://github.com/TangoSplicer/SynQ/actions/runs/31847601825 "SynQ Compiler Core #31"
[7]: https://github.com/TangoSplicer/SynQ/actions/runs/31848161711 "SynQ Compiler Core #35"
[8]: https://github.com/TangoSplicer/SynQ/actions/runs/31848568933 "SynQ Compiler Core #36"
[9]: https://github.com/TangoSplicer/SynQ/actions/runs/31848936812 "SynQ Compiler Core #37"
