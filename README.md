# SynQ

> **SynQ is an experimental hybrid quantum–classical language kernel.** Its
> currently verified surface is a deliberately bounded C++ recovery compiler,
> `synqc 0.1.0-experimental`, an experimental opaque C ABI/native SDK path, and
> a public evidence-ledger showcase. It is not a production language release,
> quantum-hardware client, general runtime, or stable cross-language SDK.

## Verified baseline

The latest independently validated compiler baseline is [Compiler Core
platform-matrix run #32374149046](https://github.com/TangoSplicer/SynQ/actions/runs/32374149046),
which passed all seven independently scoped jobs for revision `8fc1de5`:
**47/47** ordinary Ubuntu recovery-profile CTests, **33/33** Windows MSVC and
macOS Clang platform-neutral CTests, three clean-install static-SDK consumer
checks, and an additive **33/33** Ubuntu Clang ASan/UBSan core profile. The new
deterministic replay target uses a checked-in five-source SynQ corpus and eight
fixed single-byte mutation seeds; it is not continuous fuzzing, a security
certification, or a non-Linux sanitizer claim. The tested environment and exact
clean-install boundaries are documented in
[`docs/TESTED_ENVIRONMENTS.md`](docs/TESTED_ENVIRONMENTS.md).

| Area | Verified experimental capability | Explicit boundary |
| --- | --- | --- |
| Compiler and CLI | C++17 recovery build produces `libsynq_lib.a` and `synqc 0.1.0-experimental`, including explicit bounded `--eval-state` and U5/U6 `--eval-runtime`. | No general executor, REPL, package manager, or stable CLI contract. |
| Source profile | Typed gates, bounded declarations, Alpha Boolean/integer forms, immutable declaration evaluation, gated `var`/`set` state, qubit declarations, named measurement provenance, U3 source-only quantum routines, U4 one-pair feedback, U5 one-formal local functions, and U6 two-formal same-type local functions. | No lexical scopes beyond one non-capturing local frame, general returns, recursion, nested calls, mixed-type or arbitrary-arity calls, general control/measurement bodies, target-side state lowering, routine execution, routine ABI execution, or general runtime semantics. |
| Named registers | Alpha `name[index]` operands resolve against earlier declared registers, lower through strict Hybrid OpenQASM, and participate in bounded declaration-order local simulation. | No resource lifetime, deallocation, aliasing, dynamic allocation, or hardware mapping. |
| Control lowering | Strict Hybrid OpenQASM lowers documented literal and immutable-Boolean `if` forms, plus one U4 top-level named measurement immediately followed by `if <same-result> then quantum x register[index]`. | No nested negation, binary expression, alias, `else`, loop, measurement body, branch execution, or general classical-runtime lowering. |
| Local simulation | Opt-in pure-state probability calculation for explicit declared default or named registers and a limited gate set; U4 models one terminal named measurement/direct-`x` pair through deterministic branch enumeration. | No sampled host value, general collapse API, noise, general control execution, providers, hardware execution, or general resource lifecycle. |
| Semantic inspection | `synqc --inspect-semantics` renders resolved immutable-binding and mutable-cell metadata. | No runtime values, nested scopes, general type system, or execution behavior. |
| Native interoperability | The Ubuntu full profile exercises C, direct Rust, test-only Common Lisp, test-only Clojure/JNA, test-only Mercury, and a source-only Alpha Rust wrapper over the opaque C ABI; Windows and macOS run independent platform-neutral compiler/CLI/C-ABI smoke coverage. Ubuntu 22.04, Windows MSVC, and macOS Clang also prove static SDK clean-install consumers. | The Alpha Rust wrapper is not a registry package or cross-platform delivery claim. There is no shared-library distribution, frozen ABI, or general cross-platform SDK guarantee. |

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
| [`docs/NEGATED_IDENTIFIER_IF_LOWERING.md`](docs/NEGATED_IDENTIFIER_IF_LOWERING.md) | Alpha negated identifier-if target-storage and source-lowering contract. |
| [`docs/LITERAL_NEGATION_IF_LOWERING.md`](docs/LITERAL_NEGATION_IF_LOWERING.md) | Alpha compile-time literal-negation if source-lowering contract. |
| [`docs/BOUNDED_CALLABLE_KERNELS.md`](docs/BOUNDED_CALLABLE_KERNELS.md) | Alpha non-recursive one-gate kernel and source-only call-expansion contract. |
| [`docs/BOUNDED_RECURSIVE_CONSTANT_EVALUATION.md`](docs/BOUNDED_RECURSIVE_CONSTANT_EVALUATION.md) | Alpha bounded immutable Boolean and Integer declaration-expression evaluation contract. |
| [`docs/BOUNDED_MUTABLE_CLASSICAL_STATE.md`](docs/BOUNDED_MUTABLE_CLASSICAL_STATE.md) | Remotely validated Alpha U2 local mutable-cell and assignment contract, including strict non-runtime and non-target boundaries. |
| [`docs/BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md`](docs/BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md) | Remotely verified U3 Alpha explicit qubit/angle routine and static source-expansion contract. |
| [`docs/BOUNDED_MEASUREMENT_FEEDBACK.md`](docs/BOUNDED_MEASUREMENT_FEEDBACK.md) | Remotely verified U4 Alpha named-measurement/direct-`x` feedback, strict source-lowering, and deterministic local branch-enumeration contract. |
| [`docs/BOUNDED_CLASSICAL_CALLABLE_EXECUTION.md`](docs/BOUNDED_CLASSICAL_CALLABLE_EXECUTION.md) | Remotely verified U5 Alpha one-formal local callable-evaluation contract with explicit evaluator, source, simulator, and ABI boundaries. |
| [`docs/CLASSICAL_CALLABLE_RUNTIME_TUTORIAL.md`](docs/CLASSICAL_CALLABLE_RUNTIME_TUTORIAL.md) | Beginner-first U5 local callable tutorial and safe failure explanation. |
| [`docs/BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md`](docs/BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md) | Remotely verified U6 Alpha two-formal same-type local callable contract with strict parser, resolver, evaluator, source, simulator, and ABI boundaries. |
| [`docs/BINARY_CLASSICAL_CALLABLE_RUNTIME_TUTORIAL.md`](docs/BINARY_CLASSICAL_CALLABLE_RUNTIME_TUTORIAL.md) | Beginner-first U6 two-input local callable tutorial and safe failure explanation. |
| [`docs/CONFORMANCE_FIXTURE_MANIFEST_v0.1.0.md`](docs/CONFORMANCE_FIXTURE_MANIFEST_v0.1.0.md) | Versioned index of 15 pinned entry fixtures, their CTest owners, profile boundaries, and direct raw-source downloads. |
| [`docs/CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md`](docs/CONFORMANCE_REQUIREMENT_MATRIX_v0.1.0.md) | Initial requirement-to-test mapping for the U1–U6 candidate subset, including named closure gaps. |
| [`docs/ZERO_COST_SANITIZER_HARDENING.md`](docs/ZERO_COST_SANITIZER_HARDENING.md) | Remotely verified additive Linux/Clang ASan/UBSan core-profile evidence and its explicit exclusions. |
| [`docs/DETERMINISTIC_PARSER_RUNTIME_REPLAY.md`](docs/DETERMINISTIC_PARSER_RUNTIME_REPLAY.md) | Remotely verified fixed-corpus parser/lowering/resolution/bounded-evaluation replay contract, exact seeds, reproduction command, and exclusions. |
| [`docs/RUNTIME_SUBSET_CLAIM_STANDARD.md`](docs/RUNTIME_SUBSET_CLAIM_STANDARD.md) | Zero-cost, learnability, differentiation, security, and evidence gates for any future named runtime-subset claim. |
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
| [`docs/UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md`](docs/UNIQUENESS_AND_LANGUAGE_USAGE_AUDIT_2026-08-20.md) | Evidence-backed comparison with selected quantum ecosystems, the bounded SynQ differentiation position, and a SynQ-native learning-material policy. |
| [`docs/KNOWN_LIMITS_v0.1.0.md`](docs/KNOWN_LIMITS_v0.1.0.md) | Versioned ledger of unsupported language, runtime, target, ABI, platform, hardware, and evidence claims. |
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
