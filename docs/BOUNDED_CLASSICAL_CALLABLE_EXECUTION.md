# Bounded Classical Callable Execution (U5)

**Feature stage:** Proposed Alpha contract for U5.  
**Status:** Design only. This document does **not** claim callable execution,
local lexical scopes, a general runtime, target-side execution, ABI execution,
or hardware behavior until the implementation and verification gate below pass.

## Goal

U5 is the smallest useful **classical runtime** increment after U4 measurement
feedback. It makes one typed, non-recursive classical function executable under
an explicit local evaluator without reinterpreting existing quantum `call`
syntax or broadening SynQ into a general-purpose runtime.

The user-facing shape is deliberately familiar and beginner-readable:

```synq
#[experimental(feature = "classical-callable-execution")]

fn increment(value: Integer) -> value + 1
let answer = increment(41)
```

The explicit feature annotation makes the experiment visible. The `fn` return
expression describes a local deterministic value transformation; it is not an
OpenQASM subroutine, a target-side function, a native callback, or a cloud/API
call.

## Accepted U5 source subset

Exactly the following forms are in scope.

| Source element | Accepted shape | Boundary |
| --- | --- | --- |
| Function declaration | `fn <name>(<parameter>: <type>) -> <body>` | Exactly one parameter; function appears before every invocation; no overloads. |
| Parameter type | `Integer`, `Boolean`, or `String` | No Decimal, inferred types, qubit types, containers, union types, or foreign values. |
| Integer body | `<parameter>`, or `<parameter> <+|-|*> <integer-literal>` | One formal reference and, at most, one checked `int64` operation. |
| Boolean body | `<parameter>` or `not <parameter>` | No `and`, `or`, calls, aliases, measurement results, or branch body. |
| String body | `<parameter>` | No interpolation, concatenation, encoding conversion, or I/O. |
| Invocation expression | `let <binding> = <earlier-function>(<one-actual>)` | Actual is one matching literal or one earlier immutable binding; no nested calls. |

`call name(...)` remains reserved for existing bounded **quantum routine**
source expansion. U5 uses ordinary expression-shaped function invocation only
on the right side of a top-level immutable `let`. This preserves the visible
classical/quantum boundary and prevents accidental execution of a quantum
routine.

## Runtime model

The feature uses one local call frame containing exactly the declared formal.
The body may read that formal and no other name. Therefore it cannot capture a
top-level binding, mutate caller state, allocate a qubit, invoke a second
function, access a measurement result, perform I/O, access a network, read a
secret, or submit a hardware job.

The call evaluates its actual first, validates its exact static type, binds it
to the formal, evaluates the single expression with checked arithmetic, and
returns one `BoundedValue`. The returned value becomes the immutable `let`
binding. Calls are source-order only and have no ambient state.

| Resource | Default proposed bound | Exhaustion behavior |
| --- | --- | --- |
| Function declarations | 32 | Deterministic diagnostic; no partial evaluation. |
| Function invocations | 128 | Deterministic diagnostic; no partial evaluation. |
| Call depth | 1 | Nested/direct recursion is rejected statically. |
| Expression depth | 16 | Deterministic diagnostic. |
| Operations | 128 per evaluation request | Deterministic diagnostic. |
| Integer range | Signed `int64` | Overflow is a deterministic error. |

The evaluator is invoked only by a new explicit `synqc --eval-runtime` option.
Parsing a U5 program, emitting source, or using the ABI does not execute it.

## Required gates and diagnostics

The source must enable `classical-callable-execution`. U5 intentionally does
not require the older declaration-only `callable-declarations` annotation: the
two gates cover distinct source forms. The implementation must ensure that
direct programmatic parser configuration cannot bypass the U5 gate.

The implementation reserves these diagnostic families after collision checking
against the current registry:

| Code | Condition |
| --- | --- |
| `SYNQ-P019` | Malformed U5 function declaration or unsupported typed body. |
| `SYNQ-P020` | Malformed U5 invocation expression. |
| `SYNQ-R009` | Invocation has no earlier U5 function or has an invalid result/capture relation. |
| `SYNQ-R010` | Actual/body static type or formal-reference rule is violated. |
| `SYNQ-E011` | Runtime evaluation requires explicit opt-in. |
| `SYNQ-E012` | Callable declaration, invocation, or call-depth bound is exceeded. |
| `SYNQ-E007` | The inherited checked-expression operation budget is exceeded. |
| `SYNQ-E013` | Runtime evaluator encounters an invalid internal U5 frame/body shape. |
| `SYNQ-E014` | Runtime evaluator receives a non-U5 or otherwise unsupported node. |

## Explicit non-goals

U5 does **not** introduce local `var`/`set`, top-level-state capture, closures,
multiple parameters, overloading, default arguments, named arguments, recursion,
mutual recursion, nested calls, return statements, function statements,
`if`/`else`, loops, measurement-result arguments, quantum-resource arguments,
quantum execution, OpenQASM callable definitions, target-side execution, ABI
execution, foreign calls, package management, hardware access, AI access, or
network access.

The strict Hybrid OpenQASM exporter, bounded quantum simulator, and experimental
C ABI must each explicitly reject U5 nodes. This is a security and clarity
boundary, not an implementation omission.

## Security, zero-cost, and learnability requirements

The implementation uses the project’s local C++ evaluator and free Compiler
Core workflow only. It must not introduce a paid dependency, service account,
provider key, hardware requirement, external interpreter, or mandatory new
language runtime.

Every accepted U5 form must have a minimal positive example, a malformed-source
example, a type-mismatch example, a call-before-definition example, a nested
call/recursion rejection example, a bound-exhaustion example, and a clear CLI
example. The documentation must explain that `increment(41)` is local bounded
evaluation and show its expected value; it must never imply circuit execution or
hardware submission.

The exact local frame, opt-in, no-capture rule, checked arithmetic, resource
limits, and downstream rejection behavior are U5’s differentiated hybrid safety
properties. They make the classical meaning inspectable beside quantum source
composition rather than silently delegating it to a host language.

## Implementation and verification gate

Before U5 is described as available, the implementation must provide all of the
following:

1. Typed AST/Hybrid IR provenance for the U5 declaration, body, invocation, and
   result binding without weakening existing `fn`, `kernel`, or quantum `call`.
2. Parser/feature-gate/resolver coverage for every accepted and rejected shape.
3. A separate bounded runtime evaluator, `--eval-runtime` CLI output, semantic
   inspection treatment, and explicit exporter/simulator/ABI rejection.
4. Focused C++ smoke, CLI, C ABI boundary, parser/runtime, compatibility, and
   independent no-new-service tests.
5. At least one beginner guide example and one safe-failure explanation.
6. A full local recovery suite followed by the existing six-job Compiler Core
   matrix. Only after all six jobs pass may repository status, changelog,
   readiness records, and the Evidence Ledger call U5 remotely verified.

## References

[1]: [SynQ Runtime-Subset Claim Standard](./RUNTIME_SUBSET_CLAIM_STANDARD.md)  
[2]: [SynQ U2 bounded mutable classical state](./BOUNDED_MUTABLE_CLASSICAL_STATE.md)  
[3]: [SynQ U3 bounded parameterized quantum routines](./BOUNDED_PARAMETERIZED_QUANTUM_ROUTINES.md)  
[4]: [SynQ U4 bounded measurement feedback](./BOUNDED_MEASUREMENT_FEEDBACK.md)
