# SynQ Conformance Fixture Manifest v0.1.0

**Manifest status:** Public, versioned source-fixture index for the experimental
recovery profile. This is an index of selected, directly downloadable checked-in
fixture sources; it is **not** a test-result artifact, a coverage percentage, a
stable conformance certification, or an executable download.

**Pinned implementation revision:** [`9a1a9b3`][1]

**Verification evidence:** [Compiler Core #32270327206][2] completed all six
jobs for the pinned implementation: **46/46** Linux recovery-profile CTests,
**32/32** Windows MSVC platform-neutral CTests, **32/32** macOS Clang
platform-neutral CTests, and experimental static-SDK external-consumer checks on
Ubuntu 22.04, Windows MSVC, and macOS Clang.

> Download links intentionally point to the pinned implementation revision, not
> `main`. That makes a downloaded source fixture reproducible even when the
> repository later changes. Downloading a fixture does not execute it; follow the
> documented local build and CTest workflow before running untrusted or modified
> source.

## How to use this manifest

Each ID is stable within this **v0.1.0** manifest. The **Source** link opens the
fixture in GitHub; **Download** returns the raw source at the pinned revision.
The CTest names are the registered owners in
[`compiler/CMakeLists.txt`][3]. “Platform-neutral” means the named fixture is
included in the remote Windows and macOS smoke profiles as well as the Linux
recovery profile; it is evidence for that narrow profile, not a support promise.
“Ubuntu full profile” means a fixture depends on tools deliberately excluded from
the Windows/macOS smoke profiles.

## Foundation and classical-runtime fixtures

| Fixture ID | What the fixture tracks | Registered CTest | Profile boundary | Source | Download |
| --- | --- | --- | --- | --- | --- |
| `CF-CORE-001` | Core parse, IR, and compiler-library smoke behavior. | `synq_core_smoke` | Platform-neutral. | [View][4] | [Download][5] |
| `CF-CLASSICAL-001` | Bounded immutable evaluator behavior and resource-limit failures. | `synq_bounded_evaluator_smoke` | Platform-neutral. | [View][6] | [Download][7] |
| `CF-STATE-001` | Alpha local-only typed `var`/`set` state evaluation and rejection boundaries. | `synq_bounded_state_evaluator_smoke` | Platform-neutral. | [View][8] | [Download][9] |
| `CF-CALL-U5-001` | U5 one-formal local callable execution, parser/resolver failures, bounds, and output-path rejection. | `synq_classical_callable_runtime_smoke` | Platform-neutral. | [View][10] | [Download][11] |
| `CF-CALL-U6-001` | U6 two-formal same-type local callable execution, overflow, deterministic failure, and output-path rejection. | `synq_binary_classical_callable_runtime_smoke` | Platform-neutral. | [View][12] | [Download][13] |
| `CF-CLI-001` | Supported `synqc` recovery CLI commands, including explicit runtime evaluation fixtures. | `synq_cli_smoke` | Platform-neutral when the recovery CLI is enabled. | [View][14] | [Download][15] |

## Quantum and hybrid fixtures

| Fixture ID | What the fixture tracks | Registered CTest | Profile boundary | Source | Download |
| --- | --- | --- | --- | --- | --- |
| `CF-QUANTUM-U3-001` | U3 bounded parameterized routine parsing, resolution, static expansion, and rejection boundaries. | `synq_parameterized_routine_smoke` | Platform-neutral. | [View][16] | [Download][17] |
| `CF-HYBRID-U4-001` | U4 named-measurement/direct-`x` feedback provenance, strict lowering, local branch modeling, and rejection paths. | `synq_measurement_feedback_smoke` | Platform-neutral. | [View][18] | [Download][19] |
| `CF-SIM-001` | Bounded ideal-state local probability simulation, including accepted and rejected source shapes. | `synq_bounded_simulator_smoke` | Platform-neutral. | [View][20] | [Download][21] |
| `CF-OPENQASM-U4-001` | U4 source fixture used by the strict-Hybrid CLI export and Linux reference-parser checks. | `synq_measurement_feedback_hybrid_openqasm_export`; `synq_measurement_feedback_openqasm3_reference_parse` | Ubuntu full profile for Python reference checks; the source itself is downloadable everywhere. | [View][22] | [Download][23] |

## ABI and language-interoperability fixtures

| Fixture ID | What the fixture tracks | Registered CTest | Profile boundary | Source | Download |
| --- | --- | --- | --- | --- | --- |
| `CF-ABI-C-001` | Experimental opaque C ABI parse/export ownership behavior and rejection of U5/U6 local-runtime nodes. | `synq_c_abi_smoke` | Platform-neutral C ABI smoke fixture. It does not establish stable ABI compatibility. | [View][24] | [Download][25] |
| `CF-ABI-RUST-001` | Direct Rust source consumer of the experimental C ABI. | `synq_rust_abi_smoke` | Ubuntu full profile only. It is not a released Rust package. | [View][26] | [Download][27] |
| `CF-ABI-LISP-001` | Common Lisp/CFFI consumer of the experimental C ABI. | `synq_common_lisp_abi_smoke` | Ubuntu full profile only. It is not a released Lisp package. | [View][28] | [Download][29] |
| `CF-ABI-CLOJURE-001` | Clojure/JNA consumer of the experimental C ABI. | `synq_clojure_jna_abi_smoke` | Ubuntu full profile only. It is not a released Clojure package. | [View][30] | [Download][31] |
| `CF-ABI-MERCURY-001` | Mercury source consumer of the experimental C ABI. | `synq_mercury_abi_smoke` | Ubuntu full profile only, with the workflow’s Mercury bootstrap. It is not a released Mercury package. | [View][32] | [Download][33] |

## Manifest boundaries and update rule

This manifest deliberately presents **15 entry fixtures** spanning the current
compiler foundation, bounded classical execution, quantum and hybrid behavior,
strict-output boundaries, and supported experimental interoperability fixtures.
It does **not** replace the full CTest registry, which contains additional
diagnostic, parser, exporter, reference-parser, installation, and regression
checks. The authoritative executable registration remains the CMake file.[3]

Any new manifest version must name its source revision, verification run, test
counts, added/removed fixture IDs, direct raw-source links, and platform scope.
It must not relabel an Ubuntu-only fixture as platform-neutral or use a source
link as evidence that the fixture passed. A fixture is shown as verified only
when its owning recovery profile is included in a completed remote Compiler Core
run for the pinned implementation.

## References

[1]: https://github.com/TangoSplicer/SynQ/tree/9a1a9b3 "SynQ implementation revision 9a1a9b3"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/32270327206 "Compiler Core #32270327206 — U6 binary callable runtime"
[3]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/CMakeLists.txt "Pinned compiler test registration"
[4]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/core_smoke.cpp
[5]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/core_smoke.cpp
[6]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/bounded_evaluator_smoke.cpp
[7]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/bounded_evaluator_smoke.cpp
[8]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/bounded_state_evaluator_smoke.cpp
[9]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/bounded_state_evaluator_smoke.cpp
[10]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/classical_callable_runtime_smoke.cpp
[11]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/classical_callable_runtime_smoke.cpp
[12]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/binary_classical_callable_runtime_smoke.cpp
[13]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/binary_classical_callable_runtime_smoke.cpp
[14]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/cli_smoke.cpp
[15]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/cli_smoke.cpp
[16]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/parameterized_routine_smoke.cpp
[17]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/parameterized_routine_smoke.cpp
[18]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/measurement_feedback_smoke.cpp
[19]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/measurement_feedback_smoke.cpp
[20]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/smoke/bounded_simulator_smoke.cpp
[21]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/smoke/bounded_simulator_smoke.cpp
[22]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/measurement_feedback_openqasm3.synq
[23]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/measurement_feedback_openqasm3.synq
[24]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/c_abi_smoke.c
[25]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/c_abi_smoke.c
[26]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/rust_abi_smoke.rs
[27]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/rust_abi_smoke.rs
[28]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/common_lisp_abi_smoke.lisp
[29]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/common_lisp_abi_smoke.lisp
[30]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/clojure_jna_abi_smoke.clj
[31]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/clojure_jna_abi_smoke.clj
[32]: https://github.com/TangoSplicer/SynQ/blob/9a1a9b3/compiler/tests/interop/mercury_abi_smoke.m
[33]: https://raw.githubusercontent.com/TangoSplicer/SynQ/9a1a9b3/compiler/tests/interop/mercury_abi_smoke.m
