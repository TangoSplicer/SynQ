# Experimental Rust Wrapper

**Status:** Source-based Alpha wrapper over the experimental SynQ C ABI v1.  
**Distribution:** Included in this repository only; it is not published to
crates.io and is not a stable Rust API.

## Purpose

The Rust wrapper turns the existing opaque C ABI ownership rules into a small
Rust API. It validates ABI major version `1`, owns a parsed opaque program through
`Drop`, frees native diagnostic/output strings exactly once after copying them,
and maps native status codes into a Rust `Status`/`Error` result. It intentionally
exposes only `parse_source` and bounded AST OpenQASM export.

| Rust API | Native C ABI call | Ownership and boundary |
| --- | --- | --- |
| `parse_source(&str)` | `synq_parse_source` | Returns a `Program` that frees its opaque handle on `Drop`; rejects interior NUL source before FFI. |
| `Program::export_openqasm3()` | `synq_export_openqasm3` | Returns a copied Rust `String`; native output/diagnostic allocations are released internally. |
| `abi_identifier()` | `synq_version` | Reads the documented static ABI identifier; caller never frees it. |

## Local source consumption

The crate is at `compiler/bindings/rust/synq-alpha` and has no registry
dependencies. Its build script requires `SYNQ_FFI_DIR` to point at a directory
containing the test/build C ABI shared library (`libsynq_ffi.so` on the validated
Linux path), then links only that opaque-ABI library. The Compiler Core Linux
recovery profile runs `cargo test --offline` through CMake with this environment
and its shared-library search path.

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
SYNQ_FFI_DIR="$PWD/compiler/build" \
  LD_LIBRARY_PATH="$PWD/compiler/build${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" \
  cargo test --manifest-path compiler/bindings/rust/synq-alpha/Cargo.toml --offline
```

This is a source-based conformance path, not a package-manager installation
promise. Consumers remain responsible for matching the documented native build,
toolchain, and ABI boundary.

## Explicit non-goals

The wrapper does not expose internal C++ types, a general SynQ runtime, local
simulation, semantic inspection, named-register allocation, providers, hardware,
async jobs, callbacks, panics across FFI, stable semver guarantees, or a released
crate registry package. Any native ABI change requires a wrapper compatibility
review before a claim of continued conformance.
