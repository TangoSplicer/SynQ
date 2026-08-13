# SynQ Clojure/JNA Consumer Proof

**Status:** Remotely validated test-only Clojure/JNA consumer proof in
[Compiler Core #19][2]; this is not a distributed language binding.
**Last reviewed:** 13 August 2026

## Purpose

SynQ’s opaque experimental C ABI is already exercised by C, Rust, and a
test-only Common Lisp CFFI consumer. Clojure runs on the JVM and can use Java
libraries through normal interop [1]. This increment uses the free Java Native
Access (JNA) library to load the same build-directory `libsynq_ffi.so` test
artifact, avoiding an untested direct Clojure-to-C claim or a new JNI facade.

> **Design rule:** invoke the existing opaque C ABI through JNA only. Do not
> define a Clojure package, Java API, JNI bridge, callbacks, native-image path,
> distributed artifact, or portable JVM binding policy.

## Test-only boundary

| Element | Implemented test behavior | Explicit boundary |
| --- | --- | --- |
| Runtime | Debian Clojure 1.11.1 runs a script with Debian JNA 5.14.0 on its classpath. | No `deps.edn`, Leiningen project, Maven artifact, or user-installed library. |
| Native load | JNA `NativeLibrary` receives CMake’s absolute test-library path. | No platform-independent loading scheme, search-path policy, or installed native artifact. |
| ABI declarations | The script dynamically calls ABI version, parse-source, export, and free functions with C-compatible pointer references. | No generated Java interface, reflection API, callbacks, AST access, or safe managed wrapper. |
| Functional smoke flow | Validate ABI v1; parse/export an in-memory measurement source; release opaque/string allocations; check `<memory>` `SYNQ-P008`. | No execution, provider submission, result evaluation, JVM-side compiler implementation, or hardware access. |

## Consumer and CI contract

The Clojure script uses JNA `PointerByReference` values for opaque program and
returned-string outputs. It reads returned UTF-8 strings before calling the
public string-free function and never dereferences `synq_program*`. CMake passes
the library path after the script name and registers it as CTest; compiler-core
CI installs `clojure` and `libjna-java` from APT, then runs the same test.

## Explicit non-goals

This proof does not ship a Clojure namespace or library; specify a stable JNA
interface; use JNI, Java Foreign Function and Memory APIs, GraalVM native image,
or reflection-based package discovery; guarantee portability across JDKs,
operating systems, or Clojure runtimes; or establish Clojure, Java, or JVM
interoperability beyond this one tested consumer flow.

## Verified evidence and exit criteria

The local cross-project validation passed all 12 compiler-core CTest checks,
33 frontend tests, the frontend type check and production build, the backend
health smoke test, and `git diff --check`. Compiler Core #19 then completed
successfully for commit `cccf61b`, installing Clojure/JNA and reporting the same
12/12 CTest pass, including `synq_clojure_jna_abi_smoke`.[2]

This establishes only the tested Clojure/JNA opaque in-memory
parse/export/error ownership flow for the CMake-build-directory Linux artifact.
Documentation must call this a **Clojure/JNA consumer proof**, not a Clojure
binding or full JVM interoperability.

## References

[1]: https://clojure.org/reference/java_interop "Clojure Java Interop"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31726350746 "SynQ Compiler Core #19"
