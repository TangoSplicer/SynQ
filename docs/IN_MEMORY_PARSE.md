# SynQ In-Memory Source Parsing Contract

**Status:** Locally verified native interoperability increment; remote CI
evidence for `synq_parse_source` is pending publication.
**Last reviewed:** 13 August 2026

## Purpose

The existing `synq_parse_file` C ABI proves that native callers can create an
opaque SynQ program handle, but it forces a caller to create a temporary source
file. That is a poor default for FFI callers that already hold generated text in
memory. This increment adds one deliberately narrow operation:

```c
synq_status synq_parse_source(const char* utf8_source,
                              synq_program** out_program,
                              char** out_diagnostic);
```

> **Design rule:** parse the same bounded recovery grammar from one
> NUL-terminated in-memory text buffer. Do not introduce an evaluator, REPL,
> streaming protocol, callback, virtual filesystem, embedded-NUL transport, or
> source lifetime retained by the program handle.

## Contract

| Concern | `synq_parse_source` behavior | Deliberate boundary |
| --- | --- | --- |
| Input | Reads a caller-owned NUL-terminated byte sequence treated as UTF-8 source text. The library copies/parses it during the call. | There is no length parameter, byte-slice API, embedded-NUL support, or independent UTF-8 validation pass. |
| Grammar | Uses the same parser-stream path and recovery syntax as `synq_parse_file`. | No new syntax, module/import behavior, or result evaluation is added. |
| Successful output | Creates the same opaque `synq_program*` ownership result as file parsing. | The caller cannot inspect AST objects or retain source-buffer references through the ABI. |
| Failure | Returns `SYNQ_STATUS_PARSE_ERROR` and a library-owned formatted `SYNQ-P`/`SYNQ-S` diagnostic for source failures. | A file-open `SYNQ-P001` error does not apply to in-memory text. |
| Invalid arguments | A null source or null program-output pointer returns `SYNQ_STATUS_INVALID_ARGUMENT`; diagnostic output follows the existing optional-pointer rules. | The current ABI does not expose structured machine-readable diagnostics to C/Rust. |
| Source location | Parser diagnostics use the synthetic source label `<memory>` and the existing one-based line/column model. | There is no caller-selected URI/file identifier or byte-offset mapping. |

The function is additive within the experimental ABI v1 surface. It does not
claim that ABI v1 is frozen, shared-library-distributed, or safe for arbitrary
binary compatibility across platforms.

## Implementation and test boundary

The parser now uses one internal stream-based routine for both file and memory
entry points. `parseFileWithDiagnostics` remains behaviorally compatible: only
it opens a file and produces `SYNQ-P001` when opening fails. The new C ABI
function calls the in-memory parser path and uses the same opaque-handle and
diagnostic ownership helpers as `synq_parse_file`.

The compiled C consumer and the dependency-free Rust consumer now both parse a
measurement fixture from an in-memory string, export the expected classical-bit
OpenQASM source, release every returned handle/string, and verify a structured
`<memory>` grammar error. Existing file parsing remains covered separately.

## Explicit non-goals

This is not a general embedding API, source editor integration, language server,
incremental parser, network interface, REPL, C++ ABI, Rust-native API, Mercury
binding, Common Lisp binding, Clojure/JNI bridge, execution API, or hardware
submission path.

## Exit criteria

File and memory parsing now share the documented grammar path; C and Rust
consumers compile and execute the memory parse/export and diagnostic flows; and
all existing compiler/frontend/backend/external OpenQASM checks pass locally.
Compiler-core CI must validate the expanded profile before this increment is
described as remotely verified. Documentation calls this **bounded in-memory
C-ABI parsing**, not a full language embedding interface.
