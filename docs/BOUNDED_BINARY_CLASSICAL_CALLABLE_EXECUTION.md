# Bounded Binary Classical Callable Execution (U6 Design Contract)

**Feature stage:** Remotely verified Alpha U6 contract for revision `9a1a9b3`.
**Current baseline:** [Compiler Core #32270327206][1] passed all six jobs for
this implementation revision: 46/46 Linux recovery CTests, 32/32 Windows MSVC
and macOS Clang platform-neutral CTests, and clean-install experimental
static-SDK consumer jobs on Ubuntu 22.04, Windows MSVC, and macOS Clang.

## Purpose

U6 is the smallest safe extension of U5’s one-formal local callable frame. It
adds **exactly two ordered classical inputs** to a single, non-recursive local
function, while preserving explicit opt-in, bounded resources, typed AST/IR
provenance, deterministic source-order evaluation, checked arithmetic, and
rejection in every quantum-target path.

The goal is not general functions, closures, or hybrid target execution. It is
a beginner-readable way to express a small pure calculation locally:

```synq
#[experimental(feature = "classical-callable-execution")]
#[experimental(feature = "multi-formal-classical-callables")]

fn add(left: Integer, right: Integer) -> left + right
let answer = add(20, 22)
```

`answer` has the local bounded value `Integer:42` only when the user explicitly
runs `synqc --eval-runtime`. Parsing, validation, semantic inspection, source
export, C ABI use, simulation, and hardware/provider paths do not execute this
function.

## Why this increment comes next

The runtime-subset graduation sequence says U5’s one-formal callable is only a
partial bounded-callable stage; wider arguments, returns, scopes, and effects
need independent contracts.[2] U6 addresses only the first missing element:
ordered, exact-type **binary arguments**. It intentionally defers lexical
scopes, statement bodies, returns, nesting, and all quantum-resource effects.

This focus is conservative relative to broader ecosystems. Q# documents general
operations and functions alongside execution and hardware workflows, while QIR
adaptive profiles can support IR-defined functions, classical computations, and
control flow depending on the selected profile.[3][4] SynQ must not claim feature
parity. Its distinct, testable constraint is a reviewable source-to-local-frame
contract with checked `int64` arithmetic, fixed limits, dual explicit gates, and
mandatory export/simulator/ABI rejection.

## Accepted U6 source subset

U6 requires **both** experimental annotations:

1. `classical-callable-execution` enables the U5 local-runtime family.
2. `multi-formal-classical-callables` explicitly enables this U6 extension.

Direct parser configuration must not bypass either gate. Existing U5 programs
remain valid with their original single gate and original one-formal grammar.

| Source element | Exact accepted form | Required rejection boundary |
| --- | --- | --- |
| Function declaration | `fn <name>(<left>: <type>, <right>: <same-type>) -> <body>` | Exactly two distinct identifiers; declared before every invocation; no overloads. |
| Integer body | `<left> + <right>`, `<left> - <right>`, or `<left> * <right>` | Both formals occur once and in source order; no literal, alias, call, comparison, division, or capture. |
| Boolean body | `<left> and <right>` or `<left> or <right>` | Both formals occur once and in source order; no `not`, literal, alias, call, or capture. |
| Parameter type | Both `Integer`, or both `Boolean` | No mixed types, `String`, Decimal, inferred, qubit, container, union, foreign, or measurement-result type. |
| Invocation expression | `let <binding> = <earlier-function>(<first-actual>, <second-actual>)` | Exactly two ordered actuals, each a matching literal or an earlier immutable binding; no state, nested call, result, or quantum value. |

The word **binary** in this contract means exactly two formals and exactly two
actuals. It does not grant arbitrary arity. The parser must reject an empty,
one-formal, three-formal, duplicate-formal, malformed-type, mixed-type, or
unsupported-body U6 declaration with source provenance.

## Typed representation and deterministic semantics

The AST and Hybrid IR must record, in source order:

- each of the two formal names and their shared `Integer` or `Boolean` type;
- the U6 body operator and the two formal references it consumes;
- the two invocation actuals, their source spans, and their resolution indexes;
- the exact callable declaration and each actual-binding provenance in resolver
  output.

Runtime evaluation proceeds atomically in this order:

1. Confirm explicit `--eval-runtime` opt-in and validate global resource limits.
2. Resolve the earlier callable and verify it has the U6 two-formal shape.
3. Evaluate the first actual, then the second actual, using prior immutable
   values only; each must match its corresponding formal exactly.
4. Create one local frame containing only the two formal/value bindings.
5. Evaluate the one permitted binary body operation using checked `int64`
   arithmetic or deterministic Boolean `and`/`or` semantics.
6. Add the immutable result binding only if every preceding step succeeds.

There is no ambient lookup after the frame is created. A U6 body cannot read a
top-level immutable binding, U2 state cell, measurement result, qubit, routine,
secret, file, network, AI service, provider, or hardware resource. Integer
overflow is a deterministic failure, never target-defined behavior.

## Resource and safety limits

U6 retains U5’s per-request defaults unless an explicitly named CLI option is
documented and tested. The maximum number of formals is a fixed grammar limit,
not an unbounded option.

| Resource | U6 bound | Exhaustion or violation behavior |
| --- | --- | --- |
| Classical callable declarations | 32 | Deterministic diagnostic; no partial evaluation. |
| Function formals | Exactly 2 | Parser/resolver rejection with both source locations where applicable. |
| Callable invocations | 128 | Deterministic diagnostic; no partial evaluation. |
| Call depth | 1 | Nested/direct/mutual invocation and recursion are rejected before execution. |
| Expression depth | 16 | Deterministic diagnostic. |
| Operations | 128 per request | Deterministic diagnostic. |
| Integer range | Signed `int64` | Checked overflow is a deterministic diagnostic. |
| Mutable effects | None | U2 `var`/`set` and all local mutation are rejected. |

Each failure must preserve the existing runtime atomicity rule: no later
binding, state transition, target output, ABI output, simulator state, provider
request, or hardware request is produced.

## Required diagnostics

After collision checking against the existing registry, U6 may reserve these
new families. The final spelling and remediation text must be recorded in the
implementation tests and diagnostics guide.

| Proposed code | Condition | Beginner remediation |
| --- | --- | --- |
| `SYNQ-P021` | Malformed U6 two-formal declaration or unsupported body. | Use exactly two distinct same-type formals and one documented binary body. |
| `SYNQ-P022` | Malformed U6 two-actual invocation. | Supply exactly two ordered literal or earlier immutable actuals. |
| `SYNQ-R011` | Missing earlier U6 callable, invalid formal relation, or unsupported capture. | Define the documented binary function before its one top-level invocation. |
| `SYNQ-R012` | Actual type/position does not match the corresponding formal. | Match both arguments to the declared ordered signature exactly. |
| `SYNQ-E015` | Runtime receives malformed or non-U6 binary callable data. | Rebuild through the checked parser and resolver; do not construct runtime IR manually. |

Existing U5 opt-in (`SYNQ-E011`), bound (`SYNQ-E012`), operation-budget
(`SYNQ-E007`), and unsupported-node (`SYNQ-E014`) diagnostics remain applicable
where their present conditions still hold. A U6 implementation may not repurpose
an existing code for a materially different error without updating the
diagnostic-compatibility documentation and fixtures.

## Quantum and interoperability boundaries

U6 remains **local classical evaluation only**. The strict Hybrid OpenQASM
exporter, bounded local quantum simulator, and C ABI v1 must explicitly reject
every U6 declaration and invocation before output or execution. No U6 construct
may become a quantum-kernel formal, quantum-routine actual, measured-result
argument, feedback branch, target-side expression, QIR, provider request, or
hardware payload.

The existing C, Rust, Common Lisp, Clojure, and Mercury ABI fixtures remain
interop evidence for the opaque experimental ABI only. U6 adds no released
wrapper, registry package, stable ABI, or language-package claim.

## Verification gate before any availability claim

No U6 capability is available until all items below complete for one
implementation revision.

1. AST, Hybrid IR, parser, feature gate, resolver, evaluator, CLI, strict
   exporter, simulator, and C ABI changes preserve exact U6 provenance and
   explicit boundaries.
2. A focused C++ conformance fixture covers both positive Integer and Boolean
   calls, left-to-right actual evaluation, duplicate names, arity, types,
   capture, state/result/quantum actuals, nested calls, recursion, overflow,
   each resource limit, and atomic failure.
3. CLI smoke covers positive output and each named option/diagnostic boundary.
4. C ABI, exporter, and simulator smoke fixtures prove U6 rejection rather than
   accidental execution or lowering.
5. A beginner tutorial includes one copyable success, one safe failure, a plain
   explanation of values and effects, and an explicit non-goal statement.
6. `ctest --test-dir compiler/build --output-on-failure` passes in the local
   recovery profile.
7. The unchanged implementation revision passes all six Compiler Core jobs:
   Linux recovery, Windows MSVC smoke, macOS Clang smoke, and Ubuntu/Windows/
   macOS static-SDK clean-install consumers.
8. Only then may repository status, changelog, readiness records, and the
   public Evidence Ledger call this exact U6 slice remotely verified.

The focused U6 fixture, CLI smoke extension, C ABI boundary extension, and the
full local **46/46** recovery suite passed. The unchanged implementation
revision then passed all six remote matrix jobs, so repository status, changelog,
readiness records, and the Evidence Ledger may call this exact U6 slice remotely
verified while retaining every listed non-goal.

## Explicit non-goals

U6 does not add one-formal changes, arbitrary arity, String concatenation,
numeric conversion, decimal arithmetic, variables, assignments, lexical scopes,
captures, closures, statement bodies, returns, `if`/`else`, loops, recursion,
mutual recursion, nested calls, higher-order functions, dynamic dispatch,
foreign calls, collections, generic types, qubit/resource parameters,
measurement values, target-side control, simulator execution, ABI execution,
QIR generation, provider execution, hardware access, AI access, network access,
or any claim of a complete runtime.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/32270327206 "SynQ Compiler Core — U6 bounded binary classical callable runtime"
[2]: ./RUNTIME_SUBSET_CLAIM_STANDARD.md "SynQ Runtime-Subset Claim Standard"
[3]: https://learn.microsoft.com/en-us/azure/quantum/qsharp-overview "Microsoft Q# overview"
[4]: https://docs.quantinuum.com/nexus/trainings/notebooks/basics/qir/index.html "Quantinuum QIR overview and profile capabilities"
