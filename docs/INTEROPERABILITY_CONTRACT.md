# Experimental Interoperability Contract

**Status:** Documentation of remotely exercised consumer proofs, not a
cross-language package or stable-wrapper release.

## Contract boundary

SynQ’s current interoperability boundary is the opaque experimental C ABI in
[`compiler/include/synq/synq_ffi.h`](../compiler/include/synq/synq_ffi.h). The
header, ownership rules, ABI-major policy, and installed static SDK path are
defined by [`C_ABI.md`](./C_ABI.md),
[`EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md`](./EXPERIMENTAL_ABI_AND_DISTRIBUTION_POLICY.md),
and [`EXPERIMENTAL_NATIVE_SDK.md`](./EXPERIMENTAL_NATIVE_SDK.md).

| Consumer | Evidence status | Distribution boundary |
| --- | --- | --- |
| C | Remotely exercised public-header consumer and clean-prefix installed-SDK conformance. | Experimental static library/header/CMake package only; no stable ABI commitment. |
| Rust | Remotely exercised opaque C ABI smoke consumer. | No safe Rust wrapper or crates.io package. |
| Common Lisp | Remotely exercised test-only CFFI consumer. | No CFFI/ASDF package or supported shared-library distribution. |
| Clojure | Remotely exercised test-only JNA consumer. | No Maven/JVM package, runtime service, or stable Java facade. |
| Mercury | Remotely exercised test-only C-backend foreign-interface consumer. | No Mercury package or non-C-backend support claim. |

The consumer proofs establish that the exercised fixtures can use the current
opaque C API in the Compiler Core recovery profile. They do not establish source
language interoperability, automatic binding generation, library registry
availability, broad platform support, safe ownership wrappers, or compatibility
across future experimental revisions.

## Contribution requirements

Changes to the public header, status values, ownership functions, library
target, install rules, or CMake package require review against the experimental
ABI policy and all relevant consumer fixtures. A proposed language-specific
wrapper must first define its ownership/error mapping, build/install route,
minimum supported environments, versioning policy, and tests; it must not
relabel a build-directory test fixture as a published SDK.

## Evidence updates

When a consumer contract changes, update its focused test, the C ABI documents,
the support boundary, and `CHANGELOG.md`. Record remote CI links only after the
relevant Compiler Core run succeeds.
