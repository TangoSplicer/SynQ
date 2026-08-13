# Hybrid Example Customization Guide: From Ideas to Testable Changes

> **Status: experimental design guidance.** The hybrid example's syntax and behavior are not currently verified by an enabled SynQ command-line build. This guide describes how to turn a proposed customization into a small, reviewable implementation and test plan. It does not provide validated performance settings, production recommendations, real-hardware guidance, or domain-specific results.

## Use this guide only after reading the status record

Read [PROJECT_STATUS.md](../docs/PROJECT_STATUS.md) and [HYBRID_EXAMPLE_README.md](HYBRID_EXAMPLE_README.md) before working with the source sketch. The current recovery profile builds a core C++ library and a binary-format smoke test; it does not run `HYBRID_QUANTUM_ML_EXAMPLE.synq`.

## The evidence-first customization loop

Every proposed change should follow the same small loop. This protects the project from turning conceptual code and plausible algorithm descriptions into unsupported public claims.

| Stage | Question to answer | Required artifact | Evidence to preserve |
|---|---|---|---|
| Intent | What language or runtime behavior should change? | A one-sentence issue or pull-request goal | A clear non-marketing problem statement |
| Scope | What is the smallest source fixture that represents it? | A minimal fixture, ideally fewer than one concept at a time | The fixture and its expected result |
| Implementation | Which parser, type, IR, runtime, or library component owns the behavior? | A small source change with explicit boundaries | Linked source files and design notes |
| Verification | How can the behavior be proved or disproved automatically? | At least one passing and one failing test where practical | The command, output, test name, and environment |
| Documentation | What can now be stated publicly? | A short status update tied to the evidence | A claim no broader than the test boundary |

> Do not use a parameter change as proof of improved accuracy, convergence, speed, scalability, financial quality, molecular validity, or hardware support. Those are separate measurements and require their own reproducible methodology.

## Customization dimensions

### 1. Optimization-loop controls

Proposed controls include an iteration limit, a stopping rule, a learning-rate-like value, and an initial parameter vector. Begin with a deterministic classical reference function rather than a quantum algorithm. A first test can assert that the loop terminates at the configured limit and produces a documented diagnostic when an input is invalid.

| Proposed setting | First implementation question | First useful test | Claim it would support |
|---|---|---|---|
| Iteration limit | Is the value parsed and stored as an integer? | A fixture stops at the stated limit | “The loop honors this limit” |
| Step-size value | Is the value type-checked and passed to the optimizer boundary? | Invalid or non-finite values are rejected | “This parameter is validated” |
| Stopping rule | What observable result controls termination? | Deterministic reference function stops under an explicit condition | “This stop condition is tested” |
| Initial parameters | Are arity and numeric types defined? | Mismatched parameter vectors produce a diagnostic | “This input shape is checked” |

### 2. Circuit shape and gate descriptions

Adding qubits, layers, registers, or named gates should begin as an intermediate-representation or parser problem. Do not publish simulator memory tables, gate availability lists, provider mappings, or hardware compatibility statements unless they arise from a tested implementation.

| Proposed change | Boundary to define | Minimum fixture | Important non-claim |
|---|---|---|---|
| More qubits | Register-size syntax and bounds | Create and serialize a small register | No statement about capacity or memory use |
| More layers | Repetition structure in AST/IR | Two-layer fixture with deterministic representation | No statement about approximation quality |
| New gate name | Gate enum/API and operand arity | Valid and invalid arity tests | No statement about simulator or hardware support |
| Entangling pattern | Ordering and operand rules | Fixed two- or three-qubit IR fixture | No statement about compilation or transpilation quality |

### 3. Classical data and ML-oriented structure

List literals, weights, labels, feature maps, and loss functions are useful design terms, but they are not verified SynQ language features in this example. Implement a small type and evaluation subset first. Then add fixed test data checked against a simple classical reference before making any learning-related claim.

| Proposed concept | First check | Negative case to add | Evidence boundary |
|---|---|---|---|
| List literal | Element types and length representation | Mixed or malformed element types | Syntax and type acceptance only |
| Feature vector | Fixed dimensionality rule | Dimension mismatch | Input validation only |
| Weight update | Deterministic arithmetic semantics | Non-finite or incompatible value | Arithmetic behavior only |
| Loss function | Reference formula and tolerance | Invalid domain or unsupported operation | Formula evaluation only |

### 4. Problem encodings and named algorithms

VQE, QML, and QAOA are broad research areas rather than single functions. Each should be split into a narrow, named component with a scope that can be tested. A source file mentioning a molecule, portfolio, graph, classifier, or other domain is not evidence that the project solves a real-world instance.

| Research label | Narrow first component | Suitable early evidence | Claim to defer |
|---|---|---|---|
| VQE-style | Parameterized circuit data model | AST/IR fixture and classical reference energy stub | Molecular accuracy or chemical validity |
| QML-style | Feature-vector-to-circuit representation | Type and dimension tests | Classification accuracy or generalization |
| QAOA-style | Graph-to-cost-term representation | Deterministic serialization or cost-function test | Approximation ratio or operational optimization |

## Designing a minimal fixture

Keep each fixture small enough that its expected output fits in a test assertion. A good first fixture has one input, one circuit-oriented representation, one classical operation, and one predictable output. Avoid combining optimizer loops, stochastic measurement, external data, providers, cloud services, or domain consequences in the first pass.

For every fixture, write the following before implementation:

1. **Purpose:** The exact behavior under test.
2. **Input:** Literal source, API input, or data structure.
3. **Expected success:** An AST node, serialized representation, return value, or diagnostic.
4. **Expected failure:** A malformed input or unsupported feature and its stable error class.
5. **Command:** The exact CMake, executable, or test command used locally and in CI.
6. **Boundary:** The capabilities not established by the test.

## Measuring a customization, when implementation exists

Measurements should be introduced only after the relevant path runs reproducibly. Before publishing any number, record the full experimental context.

| Measurement category | Record at minimum | Never infer from one run |
|---|---|---|
| Runtime | Machine, operating system, compiler/build flags, input size, repetitions, and summary statistic | General performance or scalability |
| Memory | Measurement method, input, peak or sampled definition, and environment | Hardware requirements |
| Numerical result | Input data, reference method, tolerance, seed, and failure criteria | Accuracy, convergence, or scientific validity |
| Test coverage | Tool version, command, included targets, and exclusions | Whole-project coverage |
| Provider or device result | Provider, backend/device identity, configuration, date, and reproducible job metadata where allowed | Broad quantum-hardware support |

## Domain-safety boundaries

The original sketch uses examples from molecular modelling, portfolio optimization, logistics, and machine learning. Until an implementation has domain-appropriate validation, these are motivations only. They must not be used to recommend drugs, choose investments, run business operations, or represent hardware-backed results.

If a future contributor adds a real dataset, avoid storing sensitive, licensed, or personal data in the repository. Document data provenance, licensing, preprocessing, and expected limitations separately from compiler tests.

## Review checklist for a proposed customization

- [ ] The change has a specific behavior statement, not a general capability claim.
- [ ] The change touches the smallest practical source area.
- [ ] The example syntax is either supported by a test or explicitly marked as proposed.
- [ ] A deterministic success case is automated.
- [ ] An invalid or unsupported case is documented and tested where practical.
- [ ] Any random sampling records a seed and an acceptance criterion.
- [ ] No accuracy, performance, scale, provider, or production statement exceeds the evidence.
- [ ] Documentation links to the exact command and test result.

## Suggested next contribution

The highest-value next step is not to make the sketch larger. It is to choose one tiny language feature from the source—such as a register declaration or a single gate representation—and add a parser/IR fixture plus a CTest target. Once that fixture passes locally and in CI, update the project-status record with the narrow fact it establishes.

For questions or proposed fixtures, use [GitHub Discussions](https://github.com/TangoSplicer/SynQ/discussions) or open a focused [GitHub issue](https://github.com/TangoSplicer/SynQ/issues).
