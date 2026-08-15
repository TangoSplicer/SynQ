# Experimental Native SDK Design Record

**Status:** Remotely validated experimental native SDK path.
**Scope:** The recovery-profile C ABI only.
**Related policy:** [`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md)

## Purpose

The current recovery build verifies the opaque C ABI from source and produces a
test-only shared object for several interop smoke tests. That is insufficient for
an external native consumer: no installed header, library target, or package
metadata exists. This design adds a **small experimental native SDK path**
without relabeling the test-only shared library as a general binary product.

> The proposed SDK is a source-build and static-library integration path for the
> bounded C ABI. It is not a stable ABI, portable binary release, or
> language-specific package.

## Proposed delivered surface

| Artifact | Proposed installation rule | Consumer purpose | Explicit boundary |
| --- | --- | --- | --- |
| `include/synq/synq_ffi.h` | Install the existing public C header. | Compile against the opaque ABI v1 declarations. | It exposes no C++ AST/HIR/runtime API. |
| `lib/libsynq_lib.a` | Install the existing static recovery library. | Link the bounded C ABI service implementation. | It is not an ABI-stable shared-library distribution. |
| `lib/cmake/SynQ/SynQConfig.cmake` and targets file | Install a CMake package exposing `SynQ::synq_lib`. | Resolve public transitive build dependencies and include paths. | The target is experimental and is supported only for the documented clean-install test path. |
| `bin/synqc` and workflow documents | Retain the existing recovery CLI installation. | Use the bounded command-line workflow. | The CLI is independently versioned as `0.1.0-experimental`. |
| ZIP archive | Package only the configured experimental SDK/CLI install surface. | Local archive inspection and transfer. | No signing, provenance attestation, platform matrix, or registry publication follows. |

The public target must make Threads, `nlohmann_json`, and OpenSSL Crypto
available through the CMake package configuration because the installed static
library relies on them at link time. The installed target must present the public
header directory through its install interface and must not expose source-tree
private include paths.

## Deliberate exclusions

The following remain outside the experimental SDK design:

| Excluded surface | Reason |
| --- | --- |
| `synq_ffi_shared` / `libsynq_ffi.so` | It remains a build-directory-only shared object for test consumers. It is not an installed library. |
| Rust, Common Lisp, Clojure, and Mercury packages | Existing consumers prove raw opaque-ABI reachability only; each ecosystem needs its own packaging and ownership/error review. |
| Package registries and operating-system repositories | No registry metadata, publication workflow, maintainer process, signing, or supported-platform coverage exists. |
| Dynamic linking, callbacks, thread-safety guarantee, stable error protocol | None is currently specified or tested by the recovery profile. |

## Conformance strategy

The test must validate the installed artifact, not an accidental source-tree
include or build-directory library. It will:

1. Install the already configured recovery build into an isolated test prefix.
2. Configure a separate minimal C/C++ CMake consumer with
   `CMAKE_PREFIX_PATH` set only to that prefix.
3. Resolve `find_package(SynQ CONFIG REQUIRED)` and link `SynQ::synq_lib`.
4. Compile a C source that includes only `<synq/synq_ffi.h>`.
5. Run the resulting executable to exercise ABI identity, bounded in-memory
   parse/export, and explicit resource cleanup.

This proves the installed header, target metadata, static library, and required
link dependencies work together on the current Linux recovery environment. It
does **not** establish a package ABI across platforms or commits.

The conformance consumer uses the C++ linker even though its source is C. This
is necessary because `synq_lib` is currently a static library implemented in
C++; it is not a claim that the public C ABI requires C++ types or that a future
shared-library SDK must retain this linking detail.

## Local evidence

On 15 August 2026, the Release recovery profile completed **27/27** CTest
checks after adding `synq_installed_sdk_conformance`. That test installed the
current build into an isolated prefix, configured a separate C/C++ CMake
consumer using only `CMAKE_PREFIX_PATH`, resolved `SynQ::synq_lib`, compiled a C
source using only `<synq/synq_ffi.h>`, and exercised ABI identity, in-memory
parse/export, and library-owned cleanup. The local CPack ZIP inspection confirmed
the archive contains `libsynq_lib.a`, the public header, `SynQConfig.cmake`,
`SynQTargets.cmake`, the `synqc` CLI, and SDK documentation, while excluding the
test-only `synq_ffi` shared artifact.

This is Linux-local evidence for the current source revision. It is not yet a
cross-platform package test, registry release, dynamic-library distribution,
signing/provenance evidence, or ABI-stability promise.

## Remote evidence

[Compiler Core #46](https://github.com/TangoSplicer/SynQ/actions/runs/31898767207)
completed successfully for revision `1de1484`, reporting **27/27** CTest checks
including `synq_installed_sdk_conformance`. This confirms the recovery CI profile
can install the experimental static library/header/CMake package into an
isolated prefix, configure and link the separate public-header C consumer, and
run its bounded ABI parse/export/cleanup flow. It does not validate artifact
signing, provenance, registry publication, cross-platform portability, dynamic
linking, or ABI compatibility across future releases.

## Acceptance criteria

The implementation may be described as an **experimental native SDK path** only
after all conditions below are satisfied:

| Criterion | Required evidence |
| --- | --- |
| No source/build leakage | The isolated consumer configures and builds through only the installed CMake package and public header. |
| Runtime ABI exercise | The isolated executable uses v1 identity, parses a supported in-memory fixture, exports OpenQASM, and releases all returned resources. |
| Existing safety boundary retained | The regular C/Rust/Common Lisp/Clojure/Mercury smoke tests and the full recovery profile continue to pass. |
| Package scope remains truthful | Install/archive records state that the SDK is experimental, static-library based, Linux-tested only, and neither signed nor registry-distributed. |
| Evidence publication | Local command output and a successful Compiler Core run are recorded separately. |
