# Bounded Mutable Classical State

**Feature stage:** Alpha contract for U2.
**Status:** Implemented and locally validated on the recovery profile
(**38/38** CTests). Remote six-job Compiler Core confirmation is still required
before repository status, release records, tested-environment guidance, or the
public Evidence Ledger present U2 as remotely verified. The implementation does
not add target-side state lowering.

## Purpose and design position

The completed U1 increment provides deterministic evaluation of immutable
top-level Boolean declaration expressions. U2 adds the smallest useful stateful
counterpart: a source-ordered store of typed, top-level classical cells, with
one explicit whole-cell assignment statement. This is intentionally not a
general runtime. Its purpose is to make a short classical preparation sequence
inspectable and reproducible before SynQ introduces loops, branch execution,
measurement feedback, or routine parameters.

> A **mutable cell** is a named, top-level classical location with a single
> statically inferred type, an initial value, and later whole-cell replacements
> evaluated in source order.

The contract remains smaller than OpenQASM’s full classical model. OpenQASM
allows same-type assignment and specifies a broad expression, control-flow, and
runtime surface; SynQ adopts only same-type copy assignment over its existing
bounded expression trees, then rejects every wider form explicitly.[1]

| Design objective | U2 rule | Deliberate exclusion |
| --- | --- | --- |
| Preserve beginner clarity | `let` remains immutable and `var` visibly introduces a cell. | No implicit promotion from immutable declaration to mutable storage. |
| Preserve determinism | Statements execute only in source order; a right-hand side reads a snapshot before its target changes. | No concurrency, scheduler, target timing, or unspecified evaluation order. |
| Preserve static safety | Every cell has one resolved static type and every write must resolve to that exact type. | No coercions, casts, unions, dynamic values, or untyped storage. |
| Preserve boundedness | Opt-in evaluation has fixed default cell, transition, expression-depth, and operation budgets. | No unbounded programs, loops, recursion, heap allocation, or hidden side effects. |

## Surface syntax

U2 reserves one new Alpha feature gate and exactly two top-level statement
forms. The grammar deliberately keeps semicolon handling consistent with the
recovery parser’s existing optional line terminator.

```synq
#[experimental(feature = "mutable-classical-state")]

var <identifier> = <supported-expression>
set <identifier> = <supported-expression>
```

The word `let` retains its existing meaning: it creates an immutable
top-level declaration. `var` creates a mutable cell; `set` replaces the value
of an earlier mutable cell. There are no type annotations in U2: the existing
`ClassicalExpression` static-type classification determines the cell type at
its declaration and remains fixed for its lifetime.

```synq
#[experimental(feature = "mutable-classical-state")]
#[experimental(feature = "classical-control-flow")]

let enabled = true
var armed = enabled
set armed = not armed
```

The example defines one Boolean cell. The final `set` reads the prior value of
`armed`, evaluates `not armed` against that pre-write snapshot, and then
replaces `armed` with `false`. It does not execute a branch, emit target-side
storage, or affect a quantum operation.

## Static type and initializer contract

The implementation must reuse the current parser-owned expression structures
and `ClassicalStaticType` vocabulary. A valid `var` initializer resolves to
exactly one of the supported static types below. A valid `set` right-hand side
must resolve to the target cell’s exact static type.

| Static type | `var` initializer forms | `set` right-hand-side forms | U2 boundary |
| --- | --- | --- | --- |
| `Boolean` | Boolean literal; earlier immutable Boolean; earlier mutable Boolean; bounded parser-owned `not`, `and`, or `or` tree. | The same Boolean subset. | No short-circuit control flow, comparisons, aliases, parentheses grammar, or target-side execution. |
| `Integer` | Integer literal; earlier immutable Integer; earlier mutable Integer; existing bounded parser-owned one-operator arithmetic tree. | The same Integer subset. | No decimal conversion, division, compound assignment, or wider arithmetic grammar. |
| `String` | Quoted literal or direct earlier immutable/mutable String identifier. | The same String subset. | No concatenation, interpolation, indexing, or mutable aliases. |

Decimals and opaque source remain invalid cell initializers and invalid write
expressions in U2. A `let` initializer must not read a mutable cell: permitting
that form would turn a declaration into a time-dependent snapshot without a
separate lifetime and constantness contract. A mutable cell may read an earlier
immutable declaration or earlier mutable cell. Forward references are invalid.

## Lifetime, identity, and write ordering

Cells live from the point of a successful `var` declaration through the end of
the one top-level program. There are no lexical blocks, shadowing, deallocation,
or re-declaration. The existing global top-level name-uniqueness rule applies
across `let`, `var`, qubit declarations, measurements, and callable names.

An assignment target must be an earlier mutable cell. Its right-hand side is
fully resolved and evaluated using the store immediately before that `set`
statement. The target receives the resulting value only after the right-hand
side succeeds. A self-reference such as `set counter = counter + 1` is valid
within the accepted bounded Integer expression shape; `var counter = counter`
is invalid because the cell does not yet exist.

The local state evaluator processes the program from first statement to last
statement. A successful evaluation returns the final state in declaration order,
including each cell name, static type, final value, declaration span, and last
write span. If evaluation fails, it returns diagnostics and no completed state
snapshot or partial trace. This makes failure observable as a failed bounded
evaluation result rather than a partly committed runtime.

## Evaluation and resource bounds

State evaluation is a separate explicit opt-in from immutable
`--eval-constants`. The intended CLI shape is:

```bash
synqc program.synq --eval-state \
  --max-state-cells 64 \
  --max-state-transitions 128 \
  --max-expression-depth 16 \
  --max-operations 128
```

The defaults are initial Alpha ceilings, not performance claims. They must be
represented in an options structure and verified by focused tests rather than
being hidden constants.

| Bound | Default | Counted event | Failure rule |
| --- | ---: | --- | --- |
| State cells | 64 | Each accepted `var` declaration. | Reject before a result if the configured maximum would be exceeded. |
| State transitions | 128 | Each successful cell initialization and each `set` write. | Reject before a result if the configured maximum would be exceeded. |
| Expression depth | 16 | Recursive parser-owned Boolean or Integer tree depth. | Reuse the bounded expression-depth diagnostic path; no partial state result. |
| Expression operations | 128 | Boolean and Integer operators across one `--eval-state` request. | Reuse the bounded operation budget; no partial state result. |
| Integer range | signed `int64_t` | Every accepted Integer evaluation. | Reuse checked-overflow rejection; no wraparound. |

Every U2 evaluator must require the feature-gated source form and explicit
state-evaluation opt-in. The evaluator must reject an otherwise valid program
that contains quantum gates, measurements, controls, callables, or any
unsupported node, rather than silently skipping it. This keeps U2 a
deterministic classical-state path, not an implied hybrid executor.

## Required parser, IR, and resolver representation

The implementation extends the existing recovery architecture rather than
creating a second state model. The following source-aligned representations are
present in the locally validated recovery profile.

| Layer | Required U2 representation | Required rejection behavior |
| --- | --- | --- |
| Parser AST | `MutableDeclarationNode` with identifier, initializer text, classified literal/expression kind, source line, and span; `AssignmentNode` with target identifier, right-hand side, line, and span. | Reject `var`/`set` without the `mutable-classical-state` gate; reject malformed form, duplicate name, missing RHS, or non-identifier target. |
| Hybrid IR | `HybridMutableDeclaration` carrying one `ClassicalExpression`; `HybridAssignment` carrying target name and one `ClassicalExpression`. | Do not lower either node to a target instruction or treat it as a quantum operation. |
| Resolver | Semantic bindings distinguish immutable values from mutable cells; resolved assignment records the prior target binding index and RHS dependencies. | Reject an unknown, later, immutable, non-classical, or static-type-mismatched target/RHS pair. |
| State evaluator | A declaration-ordered typed store and bounded write trace. | Reject unsupported nodes and return no completed state result after any error. |
| CLI and exporters | New opt-in `--eval-state`; existing `--eval-constants`, simulation, strict-Hybrid export, and C ABI paths retain explicit state-node rejection. | Do not claim local branch execution, OpenQASM assignment lowering, ABI execution, or hardware behavior. |

`--inspect-semantics` may identify a mutable cell, static type, declaration span,
and earlier dependencies, but it must not expose runtime values. This preserves
the existing inspection command’s read-only character.

## Reserved diagnostics

The following identifiers are reserved for the U2 implementation so tests and
documentation can state failure causes precisely. The message text may be
improved during implementation, but the code and failure category must remain
stable throughout the Alpha slice.

| Code | Required failure category | User-directed resolution |
| --- | --- | --- |
| `SYNQ-P014` | `var` or `set` appears without the mutable-state feature opt-in. | Add the exact `mutable-classical-state` annotation before the gated source form. |
| `SYNQ-P015` | Malformed mutable declaration. | Use `var <identifier> = <supported-expression>`. |
| `SYNQ-P016` | Malformed assignment. | Use `set <earlier-mutable-identifier> = <supported-expression>`. |
| `SYNQ-S005` | Mutable-cell initializer has an unsupported or unresolved static type. | Use a supported Boolean, Integer, or String expression whose dependencies are earlier and typed. |
| `SYNQ-S006` | Assignment target is missing, later, immutable, or otherwise not a mutable cell. | Declare the target earlier with `var` and use its exact name. |
| `SYNQ-S007` | Assignment right-hand side static type differs from the cell type. | Assign an expression with the cell’s existing static type. |
| `SYNQ-E008` | State evaluation was requested without its explicit opt-in. | Use the documented state-evaluation mode only after reviewing its bounds. |
| `SYNQ-E009` | Cell count or state-transition limit is exceeded. | Reduce the program or set an explicit permitted limit. |
| `SYNQ-E010` | State evaluation received an unsupported non-classical or unimplemented node. | Restrict the state-evaluation input to the documented U2 classical subset. |

## Target, simulation, and hybrid boundaries

U2 makes no claim that a mutable cell is OpenQASM target storage. Although
OpenQASM supports same-type assignment, its broader model includes runtime
expressions and control structures that SynQ has not yet specified or
validated.[1] Until a later target-side storage contract exists, strict-Hybrid
OpenQASM export must reject `var` and `set` nodes with a structured diagnostic.

Likewise, the bounded quantum simulator must reject state nodes. U2 does not
add a control-flow evaluator, quantum/classical scheduling, measurement value,
measurement collapse, condition execution, or hardware submission. The first
hybrid feedback use case remains U4, after typed measurement-result storage and
its local and target-side representations have their own contract.[2]

## Explicit non-goals

This increment does not add explicit type annotations, casts, arrays, aliases,
destructuring, compound assignment, declaration shadowing, scopes, functions,
callable parameters, return values, loops, `else`, branch execution,
measurement-result values, target-side assignment lowering, simulation of
classical state, noise, providers, quantum hardware access, C ABI state
execution, package delivery, or stability promises.

## Verification gate before any availability claim

Before documentation calls U2 available, the implementation must include
positive and negative parser, AST, Hybrid IR, resolver, evaluator, CLI, and
exporter/simulator-rejection fixtures. The local matrix covers feature-gate
absence, malformed syntax, duplicate names, forward references, immutable and
unknown targets, Boolean/Integer/String type mismatches, same-cell reads,
write ordering, no partial result on failure, every stated resource bound,
integer overflow, and unchanged rejection in strict-Hybrid export and local
simulation.

The full local recovery suite passed **38/38** CTests for the implementation
revision. The existing six-job remote Compiler Core matrix must also pass. Only
then may the project status, changelog, tested-environment record,
usable-language profile, and public Evidence Ledger call the capability
verified.

## References

[1] [OpenQASM live specification: classical instructions](https://openqasm.com/language/classical.html)  
[2] [SynQ usable experimental-language profile](./USABLE_EXPERIMENTAL_LANGUAGE_PROFILE.md)  
[3] [SynQ bounded recursive constant evaluation contract](./BOUNDED_RECURSIVE_CONSTANT_EVALUATION.md)  
[4] [SynQ current project status](./PROJECT_STATUS.md)
