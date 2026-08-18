# SynQ Recovery-Profile CLI

**Status:** Remotely validated supported recovery-profile workflow, including
the bounded compile-time literal-negation `if` strict-Hybrid source-lowering
slice, in [Compiler Core platform matrix #32188899985][1].
**Last reviewed:** 17 August 2026

## Build

```bash
cmake -S compiler -B compiler/build -DCMAKE_BUILD_TYPE=Release
cmake --build compiler/build --parallel 2
```

The supported executable is `compiler/build/synqc`. It is a new
recovery-profile command and is deliberately separate from the disabled
historical runtime-dependent command-line source.

## Supported commands

| Command | Behavior | Exit codes |
| --- | --- | --- |
| `synqc file.synq --validate` | Parses, lowers to Hybrid IR, and performs bounded name/static validation. | `0` success; `3` parse error; `4` lowering/resolution error. |
| `synqc file.synq --emit-openqasm [--out output.qasm]` | Emits only the documented AST OpenQASM 3 source subset. | `0` success; `3` parse error; `5` unsupported export; `6` output-write failure. |
| `synqc file.synq --emit-openqasm-hybrid [--out output.qasm]` | Emits the strict typed Hybrid OpenQASM subset: declared registers, supported gates, unnamed measurements, literal Boolean declarations, and one Alpha literal-, compile-time `not true/false`-, earlier Boolean-literal-declaration identifier-, or `not <that identifier>`-`if` gate body. | `0` success; `3` parse error; `4` lowering/resolution error; `5` unsupported export; `6` output-write failure. |
| `synqc file.synq --inspect-semantics` | Renders resolved top-level classical binding names, kinds, static types, source lines, and earlier-binding dependencies without evaluation. | `0` success; `3` parse error; `4` lowering/resolution error. |
| `synqc file.synq --eval-constants [--max-declarations n]` | Explicitly opts into declaration-only bounded constant evaluation. | `0` success; `3` parse error; `4` lowering/resolution error; `5` evaluation failure. |
| `synqc file.synq --simulate [--max-qubits n] [--max-operations n]` | Explicitly computes bounded local basis/marginal probabilities for explicit declared registers, reporting source-register offsets and measurement provenance. | `0` success; `3` parse error; `4` lowering/resolution error; `5` simulation failure. |
| `synqc --help` | Prints usage and documented safety boundary. | `0`. |

Malformed command lines return `2`. The command prints structured parser,
lowering, resolution, and evaluation diagnostics to standard error. OpenQASM
export diagnostics are textual exporter diagnostics because that existing source
generation service has not yet adopted the structured `Diagnostic` type.

## Example

```synq
#[experimental(feature = "integer-arithmetic-expressions")]
let seed = 5
let total = seed + 4
let ready = true
```

```bash
./compiler/build/synqc constants.synq --eval-constants
# seed = Integer:5
# total = Integer:9
# ready = Boolean:true
```

### Inspect the bounded semantic environment

```synq
let seed = 5
let selected = seed
measure q[0] as observed
```

```bash
./compiler/build/synqc bindings.synq --inspect-semantics
# semantic environment: top-level immutable bindings
# binding seed | Value | Integer | line 1
# binding selected | Value | Integer | line 2 | depends-on seed
# binding observed | MeasurementResult | Boolean | line 3
```

This is source/binding metadata only. It does not evaluate unknown source text,
sample a measurement, assign a runtime result value, allocate a qubit, or execute
control flow. See [`ALPHA_SEMANTIC_KERNEL.md`](./ALPHA_SEMANTIC_KERNEL.md) for the
precise contract.

## Explicit non-goals

`synqc` does not execute quantum programs on a device, submit jobs, connect to
providers, call the disabled historical runtime, evaluate general classical
programs, execute `if`/`while`, execute callables, install packages, or
establish a stable production CLI contract. `--simulate` is a small local
probability model, not a device executor, noise model, or measurement sampler.
It maps explicit named registers into one bounded declaration-order state vector
only; it does not model resource lifetime, deallocation, aliasing, dynamic
allocation, or device placement. Its supported behavior is limited to the modes
above and their tested boundaries.

The strict Hybrid mode rejects source structures the AST exporter may otherwise
infer around: missing declarations, declaration-only classical/callable nodes,
named measurement results, invalid explicit-register operands, nested negation,
binary Boolean-expression conditions, aliases, measurement-result conditions,
`while`, and `if` measurement bodies. It accepts Alpha declared named registers,
literal `if true/false` gate bodies, compile-time `if not true/false` gate bodies,
one `if enabled` gate body, and one `if not enabled` gate body when `enabled` is
an earlier Boolean-literal declaration; it does not execute any form. Its
generated register sizes are explicit typed declaration facts rather than
operand-derived estimates.

## Focused validation

`synq_cli_smoke` writes temporary source fixtures and invokes the compiled CLI
as a separate process. It verifies successful validation, exact bounded OpenQASM
file output, deterministic constant-evaluation/simulation output, and nonzero
structured diagnostic failure. It also verifies explicit-register Bell-pair,
Alpha named-register, Alpha literal-if, bounded identifier-if, bounded negated
identifier-if, and compile-time literal-negation-if strict-Hybrid OpenQASM file
output, plus read-only semantic-environment output. The local recovery profile
and [Compiler Core platform matrix #32188899985][1] both reported **35/35** Linux
CTest checks; the separate Windows and macOS platform-neutral profiles each
reported **25/25**.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32188899985 "SynQ Compiler Core literal-negation-if platform matrix"
