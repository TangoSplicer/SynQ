# SynQ C ABI Foundation

**Status:** Remotely validated experimental foundation for C, Rust, test-only
Common Lisp, test-only Clojure/JNA, and test-only Mercury C-backend consumption,
including the 26-check recovery profile in [Compiler Core #45][16]. An
experimental static native SDK path has additionally passed **27/27** checks
locally; remote CI evidence for that increment is pending. The active
[`Experimental ABI and Distribution Policy`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md)
defines the current v1 change and delivery boundaries. This is **not** a
declaration of a frozen production ABI, a shared library distribution, or a
language-specific SDK.
**Last reviewed:** 15 August 2026

## Purpose and scope

The C ABI is SynQ’s first native interoperability contract. It lets a native
consumer compile against `compiler/include/synq/synq_ffi.h`, parse a recovery
profile source file, obtain an opaque program handle, request bounded OpenQASM
3 export, read diagnostics, and release library-owned resources. The contract
does not expose C++ AST classes, parsers, containers, exceptions, or a general
SynQ runtime.

This is the appropriate first bridge for the project’s target ecosystems. Rust
supports explicit external ABI selection; Mercury documents a C foreign
language interface for applicable C backends; CFFI calls C functions from
Common Lisp; and Clojure interoperates with Java, where JNA provides a small
locally tested direct consumer path through the existing opaque ABI.[1] [2] [3]
[4] [5]

| Contract property | Current implementation | Boundary |
| --- | --- | --- |
| ABI identifier | `synq_abi_version()` returns `SYNQ_ABI_VERSION` (`1`); `synq_version()` returns `synq-c-abi/1`. | The identifier versioned the initial contract; no long-term ABI stability policy has been released yet. |
| Parse services | `synq_parse_file()` accepts a non-empty UTF-8 path, and `synq_parse_source()` accepts one NUL-terminated in-memory source string; both return an opaque `synq_program*` on success. | They delegate to the recovery-profile parser; neither parses a complete SynQ language, retains caller source storage, or accepts embedded NUL bytes. |
| Export service | `synq_export_openqasm3()` exports the current bounded OpenQASM 3 subset. | Export remains source generation, not execution, hardware submission, or provider integration. |
| Error reporting | Every fallible service returns `synq_status`; an optional library-owned UTF-8 diagnostic explains the failure. | Diagnostics are currently concise service-level messages. Rich source spans and stable diagnostic codes are future work. |
| Resource lifetime | `synq_program_free()` releases program handles and `synq_string_free()` releases strings returned by the library. Both accept `NULL`. | Callers must not free SynQ-owned values with another allocator or retain them after release. |

## Public surface

The full declaration is the source of truth: [`compiler/include/synq/synq_ffi.h`](../compiler/include/synq/synq_ffi.h).
The function set below describes the only public native functions in this
increment.

| Function | Success result | Failure behavior |
| --- | --- | --- |
| `unsigned int synq_abi_version(void)` | Returns the ABI-major integer `1`. | Does not fail. |
| `const char *synq_version(void)` | Returns a static, NUL-terminated identifier. | Does not fail; the pointer is not caller-owned. |
| `synq_parse_file(path, &program, &diagnostic)` | Returns `SYNQ_STATUS_OK` and an opaque handle. | Returns `SYNQ_STATUS_INVALID_ARGUMENT`, `SYNQ_STATUS_PARSE_ERROR`, or `SYNQ_STATUS_INTERNAL_ERROR`; a diagnostic is supplied when requested and allocation succeeds. |
| `synq_parse_source(text, &program, &diagnostic)` | Returns `SYNQ_STATUS_OK` and an opaque handle after parsing one NUL-terminated source string. | Returns `SYNQ_STATUS_INVALID_ARGUMENT`, `SYNQ_STATUS_PARSE_ERROR`, or `SYNQ_STATUS_INTERNAL_ERROR`; diagnostics use the synthetic source label `<memory>`. |
| `synq_export_openqasm3(program, &text, &diagnostic)` | Returns `SYNQ_STATUS_OK` and a library-allocated UTF-8 OpenQASM string. | Returns `SYNQ_STATUS_INVALID_ARGUMENT`, `SYNQ_STATUS_EXPORT_ERROR`, or `SYNQ_STATUS_INTERNAL_ERROR`; no partial OpenQASM output is returned. |
| `synq_string_free(value)` | Releases a library-allocated string. | Accepts `NULL`. |
| `synq_program_free(program)` | Releases a program handle. | Accepts `NULL`. |

### Ownership rules

The caller owns the `synq_program*` only after a successful parse and must
release it exactly once using `synq_program_free()`. The caller owns any
non-`NULL` output or diagnostic string returned by the three fallible functions
and must release it exactly once using `synq_string_free()`. The library owns
the string returned by `synq_version()` permanently; the caller must neither
free nor modify it.

This design avoids allocator crossing and C++ layout coupling. It also means
that a future internal AST or HIR refactor need not break callers that stay
within this header’s functions. The opaque handle is intentionally unsuitable
for serialisation, copying, cross-process transfer, or access after a library
version change.

## Verified consumer evidence

`compiler/tests/interop/c_abi_smoke.c` is compiled as **C**, includes only the
public header, and links against `synq_lib` using the C++ linker only for the
library’s C++ implementation dependencies. It verifies ABI identification,
successful parsing of a feature-gated parameterized circuit, OpenQASM output,
resource release, a missing-file parse error, a malformed known-gate shape that
propagates `SYNQ-S002`, a duplicate declaration that propagates `SYNQ-S004`,
successful bounded measurement export, and a malformed measurement that
propagates `SYNQ-P008`. The current local extension also parses an opaque
in-memory fixture containing Alpha qubit declarations, named measurement-result
metadata, bounded control, and declaration-only callables; it then verifies that
the legacy export service returns `SYNQ_STATUS_EXPORT_ERROR` with an owned
diagnostic instead of silently lowering unsupported constructs. It does not use
C++ headers or internal types.

```bash
cd /home/ubuntu/SynQ
cmake -S compiler -B /tmp/synq-c-abi -DCMAKE_BUILD_TYPE=Release
cmake --build /tmp/synq-c-abi --parallel 2
ctest --test-dir /tmp/synq-c-abi --output-on-failure
```

The preceding delivery-baseline recovery profile reports **26/26 passing** tests,
including `synq_c_abi_smoke`, `synq_clojure_jna_abi_smoke`,
`synq_mercury_abi_smoke`, `synq_rust_abi_smoke`, the typed language-core and
CLI smoke tests, parser/exporter tests, bounded evaluation/simulation checks,
and the two independent OpenQASM downstream validations. The complete profile
passed remotely in [Compiler Core #45][16]. The earlier C/Rust in-memory profile passed remotely in [Compiler Core #17][11]
for commit `65906d0`, the later Common Lisp profile passed remotely in [Compiler
Core #18][12], and the Clojure/JNA consumer passed remotely in [Compiler Core
#19][13] for commit `cccf61b`; the Mercury C-backend consumer passed remotely in
[Compiler Core #22][14] for commit `ef0505f`. None of these results freeze the
ABI or test a distributed shared library.

The expanded C-consumer typed-construct parser/error-path check passed remotely
in [Compiler Core #40][15] with the then-complete **23/23** profile. It did not
change `SYNQ_ABI_VERSION`, expand the public functions, or establish that every
typed construct can be exported through `synq_export_openqasm3`.

## What this enables next—and what it does not

| Next adapter | Safe first implementation route | Not yet implemented or claimed |
| --- | --- | --- |
| Rust | A dependency-free `rustc` smoke consumer declares the opaque v1 C ABI directly, validates parse/export/error ownership flows, and passed remotely in [Compiler Core #16][9]. A future wrapper can introduce RAII types after a separate API review. | A Rust crate, a Rust-native ABI, automatic bindings, safe wrapper types, or an in-process C++ interface. |
| Mercury | A test-only Mercury 22.01.8 C-backend module uses `pragma foreign_proc("C", ...)` to call opaque ABI v1 functions, validates in-memory parse/export/error ownership flows, and passed remotely in [Compiler Core #22][14]. Its source-distribution SHA-512 is pinned in a cacheable CI bootstrap helper. | A Mercury package, foreign type, installed compiler, all Mercury backends, or bidirectional foreign calls. |
| Common Lisp | A test-only `libsynq_ffi.so` and SBCL/CFFI consumer load the opaque v1 C ABI from the build directory, validate in-memory parse/export/error ownership flows, and passed remotely in [Compiler Core #18][11]. | A published CFFI system, installed shared library, callbacks, or all Lisp implementations. |
| Clojure | A test-only Clojure 1.11.1/JNA 5.14.0 script loads CMake’s absolute `libsynq_ffi.so` path, calls opaque ABI v1 functions, validates in-memory parse/export/error ownership flows, and passed remotely in [Compiler Core #19][13]. | A Clojure library, Java API, JNI bridge, portable JVM binding, or installed native library. |

The C ABI is therefore **a foundation for interoperability, not proof of full
interoperability**. Each later binding must be independently built and tested,
and it may expose a smaller, safer surface than the raw C header. The policy
requires a new ABI major and updated consumer fixtures for a public-header or
ownership-breaking change; it does not promise ABI v1 stability across commits.
The separately documented experimental native SDK path installs only the static
library, header, and CMake package and has local clean-prefix evidence; it does
not convert `synq_ffi_shared` into an installed shared library.[17]

## Deliberate non-goals

This release does not install a system-wide library, publish a package, expose
callbacks, accept length-aware or embedded-NUL source buffers, guarantee thread
safety, provide stable error codes across releases, execute quantum programs,
submit to quantum hardware, or expose arbitrary SynQ AST/IR objects. A
test-only Linux shared artifact exists in the CMake build directory solely for
the Common Lisp CFFI, Clojure/JNA, and Mercury C-backend smoke tests; it is not
a distributed shared library.
Those decisions require separate design, threat modelling, API review, and
tests.

## References

[1]: https://doc.rust-lang.org/reference/abi.html "The Rust Reference: Application binary interface"
[2]: https://mercurylang.org/information/doc-release/mercury_user_guide/Foreign-language-interface.html "The Mercury User’s Guide: Foreign language interface"
[3]: https://cffi.common-lisp.dev/manual/cffi-manual.html "CFFI User Manual"
[4]: https://clojure.org/reference/java_interop "Clojure Java Interop"
[5]: https://github.com/java-native-access/jna "Java Native Access"
[6]: https://github.com/TangoSplicer/SynQ/actions/runs/31718265429 "SynQ Compiler Core #8"
[7]: https://github.com/TangoSplicer/SynQ/actions/runs/31721517239 "SynQ Compiler Core #12"
[8]: https://github.com/TangoSplicer/SynQ/actions/runs/31722554030 "SynQ Compiler Core #14"
[9]: https://github.com/TangoSplicer/SynQ/actions/runs/31723306294 "SynQ Compiler Core #15"
[10]: https://github.com/TangoSplicer/SynQ/actions/runs/31724123316 "SynQ Compiler Core #16"
[11]: https://github.com/TangoSplicer/SynQ/actions/runs/31724839027 "SynQ Compiler Core #17"
[12]: https://github.com/TangoSplicer/SynQ/actions/runs/31725431911 "SynQ Compiler Core #18"
[13]: https://github.com/TangoSplicer/SynQ/actions/runs/31726350746 "SynQ Compiler Core #19"
[14]: https://github.com/TangoSplicer/SynQ/actions/runs/31729572407 "SynQ Compiler Core #22"
[15]: https://github.com/TangoSplicer/SynQ/actions/runs/31849787206 "SynQ Compiler Core #40"

[16]: https://github.com/TangoSplicer/SynQ/actions/runs/31887461976 "SynQ Compiler Core #45"

[17]: ./EXPERIMENTAL_NATIVE_SDK.md "Experimental Native SDK design and local evidence"
