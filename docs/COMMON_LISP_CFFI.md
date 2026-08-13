# SynQ Common Lisp CFFI Consumer Proof

**Status:** Locally verified Common Lisp CFFI consumer proof; remote CI evidence
for this increment is pending publication.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s experimental C ABI now supports opaque program handles, bounded
OpenQASM export, and in-memory source parsing. Common Lisp implementations use
CFFI to call C functions, but CFFI needs a dynamically loadable native library
rather than a static archive [1]. This increment produces a **test-only** shared
library from the existing C ABI and validates one SBCL/CFFI consumer against it.

> **Design rule:** the shared artifact exists only in the CMake build directory
> for the smoke test. It is not installed, versioned for distribution, or
> advertised as a general-purpose SynQ shared-library release.

## Shared-library boundary

| Element | Implemented test-only behavior | Explicit boundary |
| --- | --- | --- |
| Library target | `synq_ffi_shared` links the existing `synq_lib` through a private C++ anchor and produces `libsynq_ffi.so` on the CI Linux profile. | No installation target, SONAME policy, Windows/macOS artifact, or cross-platform binary guarantee. |
| ABI surface | Exported calls remain only those in `synq_ffi.h`; the consumer sees opaque pointers and C primitives. | No C++ symbols, AST traversal, callbacks, or implementation-specific ownership transfer. |
| Lisp runtime | SBCL and Debian’s `cl-cffi` package load the build-directory library with CFFI. | No ASDF system, Quicklisp project, portability promise across Lisp implementations, or published package. |
| Functional smoke flow | Check ABI v1; parse/export a bounded measurement source in memory; free library allocations; check a `<memory>` `SYNQ-P008` error. | No result evaluation, circuit execution, provider integration, classical runtime, or hardware access. |

## Consumer contract

The Lisp smoke script declares the same `synq_parse_source`,
`synq_export_openqasm3`, `synq_program_free`, and `synq_string_free` functions
as CFFI foreign functions. It passes NUL-terminated source text, receives all
opaque handles and output strings through C-compatible pointers, and releases
each successful allocation exactly once. The script never dereferences
`synq_program*`, calls C++ classes, or treats a returned string as Lisp-owned.

The CMake test invokes `sbcl --noinform --script` with the generated shared
library’s absolute path. Compiler-core CI installs `sbcl` and `cl-cffi` through
APT, alongside the existing native, Python, and Rust dependencies.

## Explicit non-goals

This proof does not create a Common Lisp package; distribute an ASDF system;
support Quicklisp; define an idiomatic Lisp API; establish multi-threaded,
callback, condition/restart, garbage-collection, or implementation portability
rules; or deliver Mercury, Clojure, JVM, or any other language binding.

## Exit criteria

The shared target and SBCL/CFFI test now build and run locally; they validate
the documented opaque C ABI ownership and in-memory measurement flow; and every
compiler, frontend, backend, Rust, and independent OpenQASM check passes.
Compiler-core CI must validate this profile before the proof is described as
remotely verified. Documentation calls this a **Common Lisp CFFI consumer
proof**, not a Common Lisp binding or full interoperability.

## References

[1]: https://cffi.common-lisp.dev/manual/cffi-manual.html "CFFI User Manual"
