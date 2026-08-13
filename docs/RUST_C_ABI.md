# SynQ Rust C-ABI Consumer Proof

**Status:** Remotely validated Rust C-ABI consumer proof.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s published native boundary is a C ABI with opaque program handles and
explicit ownership functions. Rust can call a C ABI through `extern "C"`; that
is a direct, testable route from the current recovery profile to a major systems
language without exposing C++ object layout or committing to a Rust-native ABI
[1].

This increment creates one dependency-free Rust smoke consumer. It is not a
crate, package, code generator, idiomatic safe SDK, Rust compiler backend, or
claim that SynQ programs are Rust-compatible.

> **Design rule:** the Rust source declares only the released opaque C ABI
> functions. It owns `synq_program*` only after `SYNQ_STATUS_OK`, converts
> library-owned UTF-8 strings before calling the documented free function, and
> does not dereference the opaque program pointer.

## Consumer contract

| C ABI item | Rust representation | Consumer behavior |
| --- | --- | --- |
| `synq_program` | An empty opaque Rust struct used only behind `*mut`. | Never read, copied, or constructed by Rust. |
| `synq_status` | A C-compatible integer result checked against the documented numeric constants. | A non-zero parse/export result requires an allocated diagnostic when the API provides one. |
| `synq_version` / `synq_abi_version` | `extern "C"` functions returning an immutable C string / ABI major. | Verify ABI version 1 and the exact v1 identifier before exercising the smoke flow. |
| `synq_parse_file` | `extern "C"` with out-pointers. | Parse a temporary supported source file containing a measurement. |
| `synq_export_openqasm3` | `extern "C"` with out-pointers. | Verify output contains the bounded measurement/classical-bit lowering. |
| `synq_string_free` / `synq_program_free` | `extern "C"` destructor functions. | Release each library allocation exactly once; `NULL` remains acceptable. |

The source uses only Rust’s standard library and is compiled directly by
`rustc --edition=2021`. It deliberately avoids a Cargo workspace or third-party
FFI crate so this first proof has no package registry, lockfile, or paid-service
dependency.

## Validation and linking boundary

The CMake recovery profile now locates `rustc`, builds the Rust source against
the already-built static `synq_lib`, links the platform C++ runtime and the same
native dependencies required by the library, and registers the resulting binary
as a CTest check. Compiler-core CI now installs the distribution Rust toolchain
via APT before configuration so the same test can run remotely.

The smoke consumer verifies ABI v1 identity, a successful in-memory
parse/export/cleanup path for a measurement fixture, and a structured
`<memory>` measurement error. It does not rely on C++ headers, internal source
classes, the C consumer fixture, generated bindings, or an installed shared
library.

## Explicit non-goals

This increment does not publish a crate; create Rust RAII types; provide safe
wrappers; guarantee panic safety, thread safety, async behavior, callbacks, or
cross-platform binary distribution; expose a Rust-native ABI; or bind Mercury,
Common Lisp, Clojure, JVM, Python, or any other language. Each of those requires
its own API/design, toolchain test, and ownership review.

## Exit criteria

The standalone Rust source now compiles and runs through CTest locally; it
validates ABI version, measurement source export, ownership cleanup, and a
structured error; and all existing compiler, frontend, backend, and independent
OpenQASM checks pass. The same compiler profile passed in
[Compiler Core #16][2] for commit `41edd99`. Documentation calls this a **Rust
C-ABI consumer proof**, not a Rust binding or interoperability completion.

## References

[1]: https://doc.rust-lang.org/reference/abi.html "The Rust Reference: Application binary interface"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31724123316 "SynQ Compiler Core #16"
