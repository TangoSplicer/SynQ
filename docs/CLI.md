# SynQ Recovery-Profile CLI

**Status:** Remote evidence through [Compiler Core #43][1] covers validation,
AST OpenQASM export, constant evaluation, and bounded simulation. The strict
Hybrid OpenQASM CLI mode has local evidence pending publication and CI.
**Last reviewed:** 15 August 2026

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
| `synqc file.synq --emit-openqasm-hybrid [--out output.qasm]` | Emits strict typed Hybrid IR OpenQASM only with one explicit `qubit q[n]` declaration. | `0` success; `3` parse error; `4` lowering/resolution error; `5` unsupported export; `6` output-write failure. |
| `synqc file.synq --eval-constants [--max-declarations n]` | Explicitly opts into declaration-only bounded constant evaluation. | `0` success; `3` parse error; `4` lowering/resolution error; `5` evaluation failure. |
| `synqc file.synq --simulate [--max-qubits n] [--max-operations n]` | Explicitly computes bounded local basis/marginal probabilities. | `0` success; `3` parse error; `4` lowering/resolution error; `5` simulation failure. |
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

## Explicit non-goals

`synqc` does not execute quantum programs on a device, submit jobs, connect to
providers, call the disabled historical runtime, evaluate general classical
programs, execute `if`/`while`, execute callables, install packages, or
establish a stable production CLI contract. `--simulate` is a small local
probability model, not a device executor, noise model, or measurement sampler.
Its supported behavior is limited to the modes above and their tested boundaries.

The strict Hybrid mode rejects source structures the AST exporter may otherwise
infer around: missing/multiple/non-default registers, declaration-only
classical/callable nodes, named measurement results, controls, and invalid
explicit-register operands. This means its generated `qubit[n] q;` size is an
explicit typed declaration fact rather than an operand-derived estimate.

## Focused validation

`synq_cli_smoke` writes temporary source fixtures and invokes the compiled CLI
as a separate process. It verifies successful validation, exact bounded OpenQASM
file output, deterministic constant-evaluation/simulation output, and nonzero
structured diagnostic failure. The local recovery profile reported **26/26**
CTest checks. It now also verifies explicit-register Hybrid OpenQASM file output
and Bell-pair lowering. This is local evidence pending publication and
compiler-core CI.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31886881473 "SynQ Compiler Core #43"
