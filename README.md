# SynQ

> **SynQ is an experimental hybrid quantum–classical language kernel.** Its
> currently verified surface is a deliberately bounded C++ recovery compiler,
> `synqc 0.1.0-experimental`, an experimental opaque C ABI/native SDK path, and
> a public evidence-ledger showcase. It is not a production language release,
> quantum-hardware client, general runtime, or stable cross-language SDK.

## Verified baseline

The latest independently validated compiler baseline is [Compiler Core
platform-matrix run #32018610062](https://github.com/TangoSplicer/SynQ/actions/runs/32018610062),
which passed all five independently scoped jobs for revision `fb1d2b4`: **27/27**
Linux recovery-profile CTests, **20/20** Windows MSVC and macOS Clang
platform-neutral CTests, and Ubuntu 22.04 plus Windows MSVC clean-install
static-SDK consumer checks. The tested
environment and exact clean-install boundaries are documented in
[`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md).

| Area | Verified experimental capability | Explicit boundary |
| --- | --- | --- |
| Compiler and CLI | C++17 recovery build produces `libsynq_lib.a` and `synqc 0.1.0-experimental`. | No general executor, REPL, package manager, or stable CLI contract. |
| Source profile | Typed gates, bounded declarations, Alpha Boolean/integer forms, qubit declarations, named measurement metadata, and declaration-only callables. | No scopes, blocks, callable bodies/calls, general expressions, or runtime semantics. |
| Named registers | Alpha `name[index]` operands resolve against earlier declared registers and lower through strict Hybrid OpenQASM. | No resource lifetime, flattening, or multi-register simulation. |
| Control lowering | Strict Hybrid OpenQASM lowers one Alpha literal `if` with one supported typed gate body. | No identifier/expression condition lowering, loops, measurement body, branch execution, or classical runtime. |
| Local simulation | Opt-in pure-state probability calculation for one default register and a limited gate set. | No samples/collapse, noise, controls, providers, or hardware execution. |
| Native interoperability | The Ubuntu full profile exercises C, Rust, test-only Common Lisp, test-only Clojure/JNA, and test-only Mercury consumers; Windows and macOS run independent platform-neutral compiler/CLI/C-ABI smoke coverage. Ubuntu 22.04 and Windows MSVC also prove static SDK clean-install consumers. | No macOS SDK clean-install proof, safe wrappers, registry packages, shared-library distribution, frozen ABI, or general cross-platform SDK guarantee. |

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
| [`docs/C_ABI.md`](docs/C_ABI.md) | Authoritative opaque C ABI contract. |
| [`docs/INTEROPERABILITY_CONTRACT.md`](docs/INTEROPERABILITY_CONTRACT.md) | Consumer-proof and package-distribution boundaries. |
| [`docs/EXPERIMENTAL_NATIVE_SDK.md`](docs/EXPERIMENTAL_NATIVE_SDK.md) | Installed static SDK/CMake-package path. |
| [`docs/SECOND_CLEAN_INSTALL_ENVIRONMENT.md`](docs/SECOND_CLEAN_INSTALL_ENVIRONMENT.md) | Fixed Ubuntu 22.04 static-SDK conformance evidence and limits. |
| [`docs/BEGINNER_TUTORIAL.md`](docs/BEGINNER_TUTORIAL.md) | First build, validation, export, and local-probability walkthrough. |
| [`docs/BEGINNER_EXPERIENCE.md`](docs/BEGINNER_EXPERIENCE.md) | Guided-playground and visual-lesson non-execution contract. |
| [`docs/COMPETITIVE_LANDSCAPE_AND_DIFFERENTIATION.md`](docs/COMPETITIVE_LANDSCAPE_AND_DIFFERENTIATION.md) | Competition review and constrained differentiation strategy. |
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
