# SynQ Mercury C-Backend Consumer Proof

**Status:** Locally validated test-only Mercury C-backend consumer proof;
remote compiler-core evidence is pending publication of this increment.
**Last reviewed:** 13 August 2026

## Purpose

Mercury can emit C and use a C `pragma foreign_proc` implementation when the C
backend is selected [1]. SynQ uses this capability only to test the existing
opaque experimental C ABI. The proof compiles one Mercury module against
test-only CMake-build-directory `libsynq_ffi.so`; no C++ implementation detail
or alternate SynQ runtime is exposed.

> **Design rule:** call only the existing opaque C ABI from `pragma
> foreign_proc("C", ...)` bodies. Do not make `synq_program*` a Mercury type,
> dereference it, install a library, or claim support for any backend other than
> the single tested C-backend configuration.

## Test-only boundary

| Element | Implemented test behavior | Explicit boundary |
| --- | --- | --- |
| Toolchain | Mercury 22.01.8 is built from the official source distribution after its published SHA-512 digest is checked. The build is isolated outside the SynQ repository with one `asm_fast.gc` C-backend grade. | No system-wide package, compiler distribution, supported developer environment, source-to-source SynQ compiler, or guarantee for later Mercury versions. |
| Native artifact | CMake’s existing `synq_ffi_shared` target creates build-directory `libsynq_ffi.so`; Mercury links against its generated absolute path and receives a runtime search path for that same directory. | No installed library, SONAME policy, cross-platform binary, static-link guarantee, or distributed artifact. |
| Foreign boundary | The Mercury module includes `synq/synq_ffi.h` in `foreign_decl` and calls only ABI v1 functions in `foreign_proc("C", ...)` bodies. | No Mercury foreign type, manual binding package, C++ class access, callbacks, exceptions across ABI boundaries, or alternate Mercury backend. |
| Functional smoke flow | Check ABI identity; parse/export a bounded in-memory measurement source; free every returned SynQ allocation; assert the `<memory>` `SYNQ-P008` malformed-measurement diagnostic. | No execution, simulation, provider submission, observed result, classical runtime, hardware integration, or general source compatibility. |

## Consumer and build contract

`compiler/tests/interop/mercury_abi_smoke.m` contains three semidet Mercury
predicates implemented with C foreign procedures. The first verifies the ABI
integer and static identifier. The second calls `synq_parse_source`, holds the
returned `synq_program*` only in local C storage, calls
`synq_export_openqasm3`, checks two generated OpenQASM fragments, and releases
the program and any returned strings with the C ABI’s own cleanup functions.
The third checks parse-error ownership and source-specific `SYNQ-P008` text.

CMake copies the module to its build directory, invokes `mmc --make --grade
asm_fast.gc`, uses only the public C include directory, links the existing
test-only shared artifact, and registers the produced executable as
`synq_mercury_abi_smoke`. The test source is not a Mercury package and is not
installed by CMake.

The bootstrap helper `setup_mercury_toolchain.sh` is intentionally checksum
pinned and cacheable. It uses trusted Ubuntu packages for the native bootstrap
requirements (`flex` and `bison`) but does not add the Mercury package
repository because the inspected repository signing key was expired at review
time. This is an implementation-control decision, not a statement about the
security or availability of Mercury packages generally.

## Explicit non-goals

This proof does not provide a Mercury library or package; support `hlc`, Java,
C#, Erlang, .NET, or JavaScript backends; export Mercury procedures to SynQ;
define stable foreign-memory, exception, thread-safety, or cleanup policy;
install a compiler; compile arbitrary Mercury programs; or establish full
Mercury, Prolog, or logic-language interoperability.

## Local evidence and promotion rule

After the isolated source toolchain bootstrap completed, a clean CMake recovery
profile built the Mercury executable and reported **13/13** CTest checks
passing, including `synq_mercury_abi_smoke`. This evidence is local until the
same pinned bootstrap and consumer execute successfully in compiler-core CI.
Documentation must call this a **Mercury C-backend consumer proof**, not a
Mercury binding or general backend-support claim.

## References

[1]: https://mercurylang.org/information/doc-latest/mercury_user_guide/Foreign-language-interface.html "Mercury User’s Guide: Foreign language interface"
[2]: https://mercurylang.org/information/doc-release/mercury_ref/pragma-foreign_005fproc.html "Mercury Reference Manual: pragma foreign_proc"
[3]: https://dl.mercurylang.org/index.html "Mercury Downloads"
