# SynQ

> **SynQ is an experimental hybrid quantum–classical language kernel.** Its
> currently verified surface is a deliberately bounded C++ recovery compiler,
> `synqc 0.1.0-experimental`, an experimental opaque C ABI/native SDK path, and
> a public evidence-ledger showcase. It is not a production language release,
> quantum-hardware client, general runtime, or stable cross-language SDK.

## Verified baseline

The latest independently validated compiler baseline is [Compiler Core
platform-matrix run #32069791226](https://github.com/TangoSplicer/SynQ/actions/runs/32069791226),
which passed all five independently scoped jobs for revision `d1288c5`: **31/31**
Linux recovery-profile CTests, **23/23** Windows MSVC and macOS Clang
platform-neutral CTests, and Ubuntu 22.04 plus Windows MSVC clean-install
static-SDK consumer checks. The tested
environment and exact clean-install boundaries are documented in
[`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md).

| Area | Verified experimental capability | Explicit boundary |
| --- | --- | --- |
| Compiler and CLI | C++17 recovery build produces `libsynq_lib.a` and `synqc 0.1.0-experimental`. | No general executor, REPL, package manager, or stable CLI contract. |
| Source profile | Typed gates, bounded declarations, Alpha Boolean/integer forms, qubit declarations, named measurement metadata, and declaration-only callables. | No scopes, blocks, callable bodies/calls, general expressions, or runtime semantics. |
| Named registers | Alpha `name[index]` operands resolve against earlier declared registers, lower through strict Hybrid OpenQASM, and participate in bounded declaration-order local simulation. | No resource lifetime, deallocation, aliasing, dynamic allocation, or hardware mapping. |
| Control lowering | Strict Hybrid OpenQASM lowers one Alpha literal `if`, or an `if` controlled by one earlier top-level Boolean-literal declaration, with one supported typed gate body. | No Boolean-expression, alias, measurement-result, loop, measurement-body, branch-execution, or general classical-runtime lowering. |
| Local simulation | Opt-in pure-state probability calculation for explicit declared default or named registers and a limited gate set. | No samples/collapse, noise, control execution, providers, hardware execution, or general resource lifecycle. |
| Semantic inspection | `synqc --inspect-semantics` renders resolved top-level classical binding metadata. | No runtime values, nested scopes, general type system, or execution behavior. |
| Native interoperability | The Ubuntu full profile exercises C, direct Rust, test-only Common Lisp, test-only Clojure/JNA, test-only Mercury, and a source-only Alpha Rust wrapper over the opaque C ABI; Windows and macOS run independent platform-neutral compiler/CLI/C-ABI smoke coverage. Ubuntu 22.04 and Windows MSVC also prove static SDK clean-install consumers. | The Alpha Rust wrapper is not a registry package or cross-platform delivery claim. There is no macOS SDK clean-install proof, shared-library distribution, frozen ABI, or general cross-platform SDK guarantee. |

## Quick start: verified compiler path

Follow the full prerequisite list in
[`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md). On the currently
tested Ubuntu-like environment:

```bash
sudo apt-get install -y cmake g++ nlohmann-json3-dev libssl-dev
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
ctest --test-dir compiler/build --output-on-failure
./compiler/build/synqc --version
```

The resulting test count is an evidence snapshot, not a coverage percentage or
complete-language claim.

### Bounded strict-Hybrid example

```synq
#[experimental(feature = "qubit-declarations")]
#[experimental(feature = "classical-control-flow")]
qubit q[1]
if true then quantum h q[0]
```

```bash
./compiler/build/synqc example.synq --emit-openqasm-hybrid --out example.qasm
```

This emits bounded OpenQASM source with `if (true) h q[0];`. It does **not**
run a SynQ branch or submit work to a device. See
[`docs/STRICT_IF_LOWERING.md`](docs/STRICT_IF_LOWERING.md) for the exact
accepted/rejected control forms.

## Documentation map

| Document | Purpose |
| --- | --- |
| [`docs/PROJECT_STATUS.md`](docs/PROJECT_STATUS.md) | Current evidence ledger and non-claims. |
| [`CHANGELOG.md`](CHANGELOG.md) | Chronological experimental recovery changes. |
| [`docs/OPERATIONAL_READINESS.md`](docs/OPERATIONAL_READINESS.md) | Measurable readiness gates and remaining gaps. |
| [`docs/NAMED_REGISTER_OPERANDS.md`](docs/NAMED_REGISTER_OPERANDS.md) | Alpha named-register contract and simulator boundary. |
| [`docs/STRICT_IF_LOWERING.md`](docs/STRICT_IF_LOWERING.md) | Strict literal-if source-lowering contract. |
| [`docs/IDENTIFIER_IF_LOWERING.md`](docs/IDENTIFIER_IF_LOWERING.md) | Alpha identifier-if target-storage and source-lowering contract. |
| [`docs/C_ABI.md`](docs/C_ABI.md) | Authoritative opaque C ABI contract. |
| [`docs/INTEROPERABILITY_CONTRACT.md`](docs/INTEROPERABILITY_CONTRACT.md) | Consumer-proof and package-distribution boundaries. |
| [`docs/EXPERIMENTAL_NATIVE_SDK.md`](docs/EXPERIMENTAL_NATIVE_SDK.md) | Installed static SDK/CMake-package path. |
| [`docs/SECOND_CLEAN_INSTALL_ENVIRONMENT.md`](docs/SECOND_CLEAN_INSTALL_ENVIRONMENT.md) | Fixed Ubuntu 22.04 static-SDK conformance evidence and limits. |
| [`docs/BEGINNER_TUTORIAL.md`](docs/BEGINNER_TUTORIAL.md) | First build, validation, export, and local-probability walkthrough. |
| [`docs/BEGINNER_EXPERIENCE.md`](docs/BEGINNER_EXPERIENCE.md) | Guided-playground and visual-lesson non-execution contract. |
| [`docs/COMPETITIVE_LANDSCAPE_AND_DIFFERENTIATION.md`](docs/COMPETITIVE_LANDSCAPE_AND_DIFFERENTIATION.md) | Competition review and constrained differentiation strategy. |
| [`docs/COMPETITIVE_LEVERAGE_REVIEW_2026-08-17.md`](docs/COMPETITIVE_LEVERAGE_REVIEW_2026-08-17.md) | Evidence-backed three-leverage competitive direction. |
| [`docs/ALPHA_SEMANTIC_KERNEL.md`](docs/ALPHA_SEMANTIC_KERNEL.md) | Bounded top-level semantic environment and inspection contract. |
| [`docs/MULTI_REGISTER_SIMULATION.md`](docs/MULTI_REGISTER_SIMULATION.md) | Bounded named-register allocation and local-probability simulation contract. |
| [`docs/EXPERIMENTAL_RUST_WRAPPER.md`](docs/EXPERIMENTAL_RUST_WRAPPER.md) | Source-only Alpha Rust wrapper and C ABI ownership contract. |
| [`docs/LANGUAGE_WIDE_IMPROVEMENT_ASSESSMENT_2026-08-17.md`](docs/LANGUAGE_WIDE_IMPROVEMENT_ASSESSMENT_2026-08-17.md) | Prioritized language-core improvement assessment, grounded in current code and evidence. |
| [`CONTRIBUTING.md`](CONTRIBUTING.md) | Safe contribution workflow. |
| [`SECURITY.md`](SECURITY.md) | Responsible vulnerability-reporting route. |

The public Evidence Ledger is available at
<https://synqshowcase-ee5qsvs3.manus.space/>. It summarizes verified evidence
and links back to repository records; it is not a hosted compiler service.

## Contributing and licensing

Please read [`CONTRIBUTING.md`](CONTRIBUTING.md) before proposing a language or
ABI increment. Use [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions)
for questions and early design work, and [GitHub Issues](https://github.com/TangoSplicer/SynQ/issues)
for reproducible defects and bounded requests.

SynQ is dual-licensed under the repository’s [MIT license](LICENSE) and
[commercial license materials](LICENSE_COMMERCIAL.md). Read the actual terms;
this README does not alter them.
