# First Local Function: U5 Classical Callable Runtime

**Status:** Candidate tutorial for the proposed Alpha U5 feature. The source and
commands below are covered by local conformance checks, but U5 is **not remotely
verified or publicly available** until its implementation revision passes the
full six-job Compiler Core matrix.

## What you will learn

This tutorial evaluates one small classical function locally. It does not create
a quantum circuit, submit a job, contact a provider, use an AI model, or require
a paid service.

The function receives one Integer, adds one, and returns the result:

```synq
#[experimental(feature = "classical-callable-execution")]

fn increment(value: Integer) -> value + 1
let answer = increment(41)
```

The expected local result is:

```text
answer = Integer:42
```

## Zero-cost local workflow

After following the repository’s tested build instructions, save the example as
`increment.synq` and run:

```bash
synqc increment.synq --eval-runtime
```

`--eval-runtime` is explicit. Parsing the file, exporting OpenQASM, using the C
ABI, or running the quantum simulator does **not** execute this function.

## Read the line in plain language

| Source | Meaning |
| --- | --- |
| `#[experimental(...)]` | You consciously opt in to an Alpha feature that may change. |
| `fn increment(value: Integer)` | Define one local function with one named Integer input. |
| `-> value + 1` | Return the input plus one using checked signed-64-bit arithmetic. |
| `let answer = increment(41)` | Call the earlier function once and bind its returned value immutably. |

## A safe failure example

SynQ rejects a function body that tries to read a surrounding name. This keeps
the first runtime frame easy to inspect and prevents hidden capture:

```synq
#[experimental(feature = "classical-callable-execution")]

let bonus = 1
fn unsafe_increment(value: Integer) -> value + bonus
```

The parser must report `SYNQ-P019` and explain that U5 allows only the formal
plus an Integer literal in this body. Rewrite it as `value + 1`, or wait for a
separately specified later scope/capture feature.

## Current boundaries

U5 accepts one earlier function with one `Integer`, `Boolean`, or `String`
formal. Integer bodies may return the formal or add, subtract, or multiply one
literal. Boolean bodies may return the formal or negate it. String bodies return
the formal unchanged.

It does not provide multiple arguments, local mutable variables, closures,
recursion, nested calls, `return` statements, `if`/`else`, loops, measurement
arguments, qubit arguments, foreign calls, network access, file access, AI
access, OpenQASM function export, ABI execution, or hardware execution. See the
[U5 design contract](./BOUNDED_CLASSICAL_CALLABLE_EXECUTION.md) for the exact
limits and verification gate.
