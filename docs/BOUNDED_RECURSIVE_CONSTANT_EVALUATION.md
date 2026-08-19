# Bounded Recursive Constant Evaluation

**Feature stage:** Alpha  
**Status:** Implemented Alpha contract. The Boolean declaration-expression
portion is remotely validated in [Compiler Core platform-matrix run
#32239066421](https://github.com/TangoSplicer/SynQ/actions/runs/32239066421).
Integer-expression evaluation retains its separately documented bounded scope.

## Scope

This contract extends `synqc --eval-constants` from literals, aliases, and one
Integer operator to **recursive, immutable, top-level Boolean and Integer
expression trees**. It is a deterministic compile-time evaluation feature, not
a general runtime.

The parser-owned expression tree is the sole source of evaluation structure. No
source text is reparsed by the evaluator.

| Category | Accepted form | Result |
| --- | --- | --- |
| Integer | Integer literal, earlier Integer binding, and recursively nested `+`, `-`, or `*` | Checked signed 64-bit Integer value. |
| Boolean | Boolean literal, earlier Boolean binding, and recursively nested `not`, `and`, or `or` | Boolean value. |
| String | Existing quoted literal and direct earlier-binding alias only | String value. |

## Safety limits

The evaluator must require explicit opt-in and apply all limits below before it
returns a value.

| Limit | Initial Alpha value | Failure behavior |
| --- | --- | --- |
| Top-level declarations | Existing configurable limit; default `64` | `SYNQ-E001` diagnostic. |
| Expression depth | Configurable; default `16` | Bounded-evaluation diagnostic; no partial evaluation result. |
| Arithmetic/Boolean operations | Configurable; default `128` for the entire evaluation request | Bounded-evaluation diagnostic; no partial evaluation result. |
| Integer representation | Signed `int64_t` | Checked overflow yields `SYNQ-E005`; no wraparound. |
| Binding dependency | Earlier immutable declarations only | Existing unresolved/type diagnostics; no forward reference or mutation. |

Short-circuit behavior is intentionally **not** introduced by this increment:
all parser-owned Boolean operands are recursively evaluated within the defined
operation budget. This makes the initial evaluator deterministic and prevents a
runtime-style control-flow meaning from being inferred.

## Required static invariants

1. Integer operations have exactly two Integer operands; Boolean `not` has one
   Boolean operand; Boolean `and` and `or` have exactly two Boolean operands.
2. Each declaration initializer has one static type that agrees with its
   expression tree and evaluator result.
3. The resolver validates prior bindings and static types before evaluation.
4. A failed declaration rejects the complete evaluation result; the evaluator
   does not expose a partial environment.

## Explicit non-goals

This contract does not add assignment, mutable cells, lexical scopes, function
calls, loops, `else`, branch execution, measurement-result values, decimals,
floating point, target-side expression execution, quantum simulation execution,
or hardware execution. It does not change strict Hybrid OpenQASM export until a
separate target-expression lowering contract is implemented.

## Recorded Boolean-evaluation evidence

The implementation evaluates the accepted Boolean trees through
`synqc --eval-constants` after parser and resolver validation. It enforces the
default depth and operation values in this contract, reports depth exhaustion as
`SYNQ-E006` and operation exhaustion as `SYNQ-E007`, and returns no partial
evaluation environment after failure. The cited six-job run passed 37 Linux
Ctests, 26 Windows MSVC CTests, 26 macOS Clang CTests, and three clean-install
static-SDK external-consumer jobs.

## Evidence required before any further availability claim

The increment requires positive and negative parser, typed-IR, resolver,
evaluator, and CLI fixtures; depth, operation, overflow, forward-reference, and
type-mismatch coverage; a full local recovery-suite pass; and the independent
remote compiler matrix before public documentation is updated.
