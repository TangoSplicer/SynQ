# First Two-Input Local Function: U6 Binary Classical Callables

**Status:** Remotely verified Alpha U6 tutorial for revision `9a1a9b3`. The
source and commands below are covered by the 46/46 local recovery suite and the
six-job [Compiler Core #32270327206](https://github.com/TangoSplicer/SynQ/actions/runs/32270327206)
matrix (32/32 Windows MSVC and macOS Clang platform-neutral CTests plus three
clean-install static-SDK consumer jobs). This remains a local bounded subset,
not a general runtime, target executor, ABI execution path, or hardware service.

U6 adds one small local-only step after the verified U5 one-input function: a
function may read **two ordered values of the same type** and compute one value
locally. It is designed to be read before it is run. It is not a quantum kernel,
a target-side function, an ABI function, or a hardware job.

## A first example

Create `add.synq` with this exact source.

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]

fn add(left: Integer, right: Integer) -> left + right
let answer = add(20, 22)
```

Run the local evaluator explicitly:

```bash
./compiler/build/synqc add.synq --eval-runtime
```

The expected local output is:

```text
answer = Integer:42
```

The two annotations are intentional. The first enables the bounded local
callable family. The second makes the wider two-input experiment visible in the
source. Removing either one must produce a feature-gate diagnostic rather than
silently broadening function behavior.

## What the line means

The `add` declaration has two names, `left` then `right`. Both are explicitly
`Integer`. When `add(20, 22)` runs, SynQ evaluates the first actual, evaluates
the second actual, creates a local frame containing only those two bindings, and
uses checked signed 64-bit arithmetic for `left + right`. The result becomes the
immutable `answer` binding only if the whole operation succeeds.

The same shape works for one Boolean operation:

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]

fn both(left: Boolean, right: Boolean) -> left and right
let enabled = both(true, false)
```

Running `--eval-runtime` prints `enabled = Boolean:false`.

## One safe failure

The formals must have the same supported type and the body may use only the
documented two-name expression. This source deliberately attempts a capture and
must be rejected before runtime evaluation:

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]

fn unsafe(left: Integer, right: Integer) -> left + global
```

The parser must report `SYNQ-P021`. Rewrite the body as one of `left + right`,
`left - right`, or `left * right`. Do not expect a function body to read a
top-level `let`, a mutable cell, a measurement result, or any ambient value.

If a Boolean function receives an Integer actual, for example
`both(true, 1)`, parsing succeeds but resolution must report `SYNQ-R012` before
execution. Both ordered actuals must match their corresponding formal type
exactly.

## Current limits

| Boundary | Current U6 rule |
| --- | --- |
| Function inputs | Exactly two, ordered, distinct, and the same type. |
| Supported types | Both `Integer`, or both `Boolean`. |
| Integer bodies | `left + right`, `left - right`, or `left * right`. |
| Boolean bodies | `left and right` or `left or right`. |
| Actuals | Each is a matching literal or an earlier immutable binding. |
| Local frame | Contains only the two formal bindings. |
| Default request limits | 32 callable declarations, 128 invocations, call depth 1, expression depth 16, and 128 operations. |

An `int64` overflow, operation-limit exhaustion, missing earlier definition,
wrong type, nested call, malformed form, or unsupported node produces a
deterministic diagnostic and no partial evaluation result.

## What this does not do

U6 does **not** provide arbitrary function arguments, String concatenation,
variables, assignments, lexical scopes, capture, return statements, `if`/`else`,
loops, recursion, nested calls, qubit arguments, measurement-result arguments,
quantum target execution, OpenQASM function export, simulator execution, C ABI
execution, provider access, network access, AI access, or hardware execution.

The strict Hybrid exporter, bounded local simulator, and experimental C ABI must
reject U6 source rather than treat it as a target executable. Use
`--eval-runtime` only for the documented local subset.

## Where to go next

Read the [U6 design contract](./BOUNDED_BINARY_CLASSICAL_CALLABLE_EXECUTION.md)
for normative source shapes, limits, diagnostics, and verification requirements.
The [U5 tutorial](./CLASSICAL_CALLABLE_RUNTIME_TUTORIAL.md) remains the right
place to learn a one-input local function first.
