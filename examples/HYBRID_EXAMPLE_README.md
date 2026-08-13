# Hybrid Quantum–Classical–ML Example: Evidence Status and Reading Guide

> **Status: illustrative source sketch; not executable in the current verified recovery profile.** The checked-in `HYBRID_QUANTUM_ML_EXAMPLE.synq` file explores how VQE-, QML-, and QAOA-like ideas might be expressed together. Its syntax, algorithm behavior, numerical results, and performance have **not** been compiled, executed, or validated by the current project review.

## Why this status matters

SynQ's current verified boundary is a C++ core-library recovery build plus one focused binary-format smoke test. The command-line compiler is intentionally disabled in that profile, and there is no verified `synq` executable capable of parsing or running this example. Therefore, this file must not be used as an installation tutorial, reproducible benchmark, quantum-hardware workflow, or production template.

For the supported build command and the latest verification boundaries, read [GETTING_STARTED.md](../GETTING_STARTED.md) and [PROJECT_STATUS.md](../docs/PROJECT_STATUS.md) first.

| Artifact | What is present | What has been verified |
|---|---|---|
| `HYBRID_QUANTUM_ML_EXAMPLE.synq` | A substantial experimental source sketch combining named VQE, QML, and QAOA concepts | The file has not been parsed, compiled, or executed in the recovery profile |
| C++ compiler core | A static library build and binary-format code | The recovery-profile library builds and one focused smoke test passes |
| Command-line compiler | A checked-in, incomplete executable path | Not verified; disabled by default |
| Quantum execution | Source concepts and examples | No verified simulator, provider, or hardware execution |

## What the example is trying to illustrate

The example is useful as a design artifact. It groups three common hybrid-computing patterns into a single narrative, but it does not establish that SynQ currently implements the corresponding language constructs or algorithms.

| Conceptual section | Intended idea | Verification boundary |
|---|---|---|
| VQE-style workflow | A parameterized circuit, an energy-estimation routine, and a classical parameter loop | No Hamiltonian model, convergence behavior, or numerical result is verified |
| QML-style workflow | Encoding classical features, evaluating a circuit-derived quantity, and updating weights | No training routine, prediction quality, or dataset behavior is verified |
| QAOA-style workflow | Building a parameterized optimization circuit and iterating its parameters | No problem encoding, approximation behavior, or solution quality is verified |
| Hybrid integration | Passing values between classical control flow and circuit-oriented functions | The interoperability syntax and runtime semantics are not yet validated |

> The presence of a familiar algorithm name does not demonstrate a working implementation. A future claim requires a reproducible command, a focused test, expected output, and reviewable source evidence.

## How to read the `.synq` source safely

Treat every line as **proposed language and API design**, not supported user syntax. Names such as `Circuit`, `Register`, `vqe_optimization`, `quantum_classifier`, and `qaoa_circuit` express a possible target interface. They should not be copied into an application or assumed to map to a compiler API until the CLI/parser path is repaired and a compile test is added.

The numeric values, output messages, and algorithmic commentary in the source are similarly illustrative. They are not measured outputs, accuracy figures, convergence guarantees, resource estimates, or statements about real quantum devices.

## Current execution status

There is deliberately no “run this example” command in this document. The old command shape depended on a `synq` executable, which the verified recovery profile does not build. Running the supported core build only produces `libsynq_lib.a` and the `synq_core_smoke` test executable; neither one parses the hybrid example.

The current verified command sequence is:

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel
ctest --test-dir compiler/build --output-on-failure
```

This confirms the recovery profile only. It does **not** validate the hybrid example, an end-to-end compiler, a simulator, or a quantum result.

## A path to make this example testable

The example should be promoted from illustrative material only in small, independently testable steps. The following acceptance criteria are intentionally concrete so that future documentation can be factual.

| Step | Minimum implementation evidence | Minimum test evidence | Claim permitted after passing |
|---|---|---|---|
| 1. Parser fixture | Define a stable subset of SynQ syntax used by a tiny source file | Parser test produces a deterministic AST or diagnostic | “This syntax subset parses” |
| 2. Type fixture | Implement and document the smallest needed primitive and circuit types | Positive and negative type-check tests | “These types are checked” |
| 3. Interpreter or compiler path | Enable one executable route for the small subset | End-to-end test compiles and runs a fixed fixture | “This minimal program runs” |
| 4. Classical reference behavior | Write a deterministic classical reference for one calculation | Numerical outputs compared within a declared tolerance | “This reference computation is tested” |
| 5. Algorithm module | Add one narrow VQE-, QML-, or QAOA-style component | Unit tests plus an integration fixture | “This named component is implemented and tested” |
| 6. Documentation promotion | Record commands, environment, input, output, and limits | CI reproduces the required checks | Public documentation may describe the verified capability |

## Recommended first fixture

Do not begin by repairing the complete hybrid example. A small fixture provides a tractable boundary and makes failures understandable. A suitable first fixture would contain only a circuit declaration, one gate operation, one deterministic measurement placeholder, and one classical value assertion. It should avoid machine-learning loops, optimization claims, molecular models, provider integrations, or hardware-specific code.

The fixture should have an explicit expected outcome such as a serialized intermediate representation or a fixed classical result. Any test relying on random sampling should document its seed, tolerance, and failure interpretation.

## How to customize the sketch responsibly

Customization is currently a design exercise, not an execution workflow. You may edit the source to explore API ergonomics, naming, control-flow structure, or future test cases. Record the intent of each change, keep the smallest possible diff, and do not infer correctness from readability alone.

| Customization goal | Safe current use | Do not claim yet |
|---|---|---|
| More iterations or layers | A proposal for a future optimizer interface | Improved convergence, accuracy, or execution time |
| More qubits or registers | A way to discuss data-model requirements | Simulator capacity, memory use, or hardware compatibility |
| Alternate loss or cost function | A design input for a future test case | Correct optimization behavior or business outcome |
| Different gate sequence | A proposed circuit representation | Gate-set support, transpilation, or provider execution |
| Domain examples | A motivation for future work | Suitability for drug discovery, investment, logistics, or other operational decisions |

For a structured list of editable dimensions and the evidence each one would need, see [HYBRID_EXAMPLE_CUSTOMIZATION_GUIDE.md](HYBRID_EXAMPLE_CUSTOMIZATION_GUIDE.md).

## Contributor checklist for future verification

Before changing this example's status, verify each applicable item and link the evidence in the pull request or issue:

- [ ] The language syntax is accepted by an enabled parser target.
- [ ] The required runtime types and functions are implemented rather than only named in source.
- [ ] The example or a smaller fixture builds through a documented command.
- [ ] Automated tests exercise both a successful path and at least one expected failure path.
- [ ] All numeric output is produced by a test or benchmark with documented inputs.
- [ ] Resource, coverage, performance, accuracy, and provider claims are backed by reproducible evidence.
- [ ] Any use involving financial, medical, security, or operational decisions is separately reviewed and tested for that context.

## Related project material

- [Repository status record](../docs/PROJECT_STATUS.md)
- [Verified build guide](../GETTING_STARTED.md)
- [Repository issues](https://github.com/TangoSplicer/SynQ/issues)
- [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions)

The project welcomes contributions that narrow the gap between this design sketch and a reproducible, tested example.
