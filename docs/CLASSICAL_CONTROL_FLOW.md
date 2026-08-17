# Bounded Typed Classical Control Flow

**Status:** The earlier literal-condition Alpha profile passed remotely in
[Compiler Core #27][1], with a successful manual repeat in [#28][2]; the
Boolean identifier-reference extension passed remotely in [Compiler Core #29][3],
and the bounded Boolean-operator extension passed remotely in [#30][4]. A
strict identifier-`if` gate source-lowering subset passed remotely in [Compiler
Core #32069791226][5].
**Last reviewed:** 17 August 2026

## Purpose

The first classical-control-flow profile adds typed source and Hybrid IR nodes
for a deliberately small, safe control boundary. It recognizes literal Boolean
conditions or resolver-checked Boolean declaration references, plus a single
quantum-gate or measurement body. The feature is **Alpha** and therefore
requires an explicit file-scoped opt-in.

> **Design rule:** control syntax must be visible in the typed AST and Hybrid
> IR, but it must not imply loop execution, branch execution, a classical
> runtime, or result handling. The strict Hybrid exporter has separately
> documented literal- and earlier Boolean-literal-declaration identifier-`if`
> gate source-lowering exceptions.

## Alpha-gated source profile

```synq
#[experimental(feature = "classical-control-flow")]
if true then quantum h q[0]
while false do measure q[0]
let ready = true
if ready then quantum x q[0]
```

| Form | Typed representation | Explicit boundary |
| --- | --- | --- |
| `if true then quantum h q[0]` | `ClassicalControlNode { If, true, QuantumGateNode }`, then `HybridControlFlow` with a typed gate body. | Strict Hybrid OpenQASM may emit `if (true) h q[0];`; no expression condition, `else`, block, result value, or execution semantics are defined. |
| `while false do measure q[0]` | `ClassicalControlNode { While, false, MeasurementNode }`, then `HybridControlFlow` with a typed measurement body. | No iteration, termination analysis, measurement-dependent condition, returned bit, or runtime effect. |
| `if ready then quantum x q[0]` | `ClassicalCondition { IdentifierReference, "ready" }`, then a `ResolvedHybridControlFlow` only after the earlier binding resolves to static type `Boolean`. | Strict Hybrid export additionally accepts only an earlier `let ready = true/false` declaration and emits immutable generated target Boolean storage. No alias, general expression, runtime read, or execution semantics are defined. |
| `if not ready then quantum h q[0]` or `while ready and enabled do measure q[0]` | `ClassicalCondition { BooleanExpression, ... }` holding exactly one `Not` or one `And`/`Or` tree over literal or identifier leaves. | No parentheses, nesting, precedence, short-circuit behavior, value computation, or branch/loop execution semantics. |
| Missing opt-in | `SYNQ-P007` before AST construction. | The feature remains Alpha; no implicit enablement or source-level bypass exists. |
| Invalid condition/structure | `SYNQ-P009` for malformed bounded Boolean-expression or connector form. | No general expression parsing, coercion, or condition evaluation is attempted. |
| Unsupported body | `SYNQ-P010` for anything other than exactly one typed quantum gate or measurement. | No nested control flow, `print`, `delay`, `ai`, `let`, multi-statement body, or fallback instruction body is accepted. |

## Parser, IR, and resolution behavior

`ClassicalControlNode` owns one typed body node. The parser validates the body
through the same bounded gate-shape and measurement validation paths used at the
top level. The Hybrid IR copies the typed condition, body variant, and source
span. `resolve_hybrid_names` validates every identifier leaf only against an
earlier top-level declaration of static type `Boolean`, preserving ordered leaf
binding indices in `ResolvedHybridControlFlow`. It does not resolve inside
control bodies because those bodies expose no classical variable references.

The direct AST OpenQASM source exporter remains a typed-AST subset and does not
lower control nodes. The strict Hybrid exporter lowers one literal `if`, or an
`if` whose whole identifier refers to an earlier Boolean-literal declaration,
with one supported typed gate body. It rejects aliases, Boolean expressions,
measurement-result conditions, `while`, and measurement bodies. This does not
claim that SynQ, a simulator, a provider, or hardware executes these forms. See
[`IDENTIFIER_IF_LOWERING.md`](./IDENTIFIER_IF_LOWERING.md) for the exact target
storage spelling and rejection contract.

## Focused validation

`synq_classical_control_flow_smoke` verifies mandatory feature gating, typed
`if` and `while` AST construction, literal condition preservation, typed
quantum/measurement body preservation, Hybrid IR lowering, name-resolution
preservation, and `SYNQ-P009`/`SYNQ-P010` rejection paths. The separate
expression smoke covers Boolean identifier conditions and `SYNQ-R002`/`SYNQ-T001`.
The separate Boolean-expression smoke covers `not`, `and`, `or`, and `SYNQ-T002`.
The expanded **18/18** profile passed remotely in [Compiler Core #30][4]. Exact
identifier-`if` source lowering and its bounded rejections passed remotely in
[Compiler Core #32069791226][5] with **31/31** Linux checks and **23/23**
Windows/macOS platform-neutral checks.

## Explicit non-goals

This Alpha increment does not implement general boolean expressions, variable
conditions beyond whole resolved identifiers, assignment, mutable variables,
`else`, blocks, nesting, function scope, loop execution, branch execution,
termination checks, runtime values,
measurement-result values, resource liveness, control-flow graph construction,
optimization, general OpenQASM lowering, simulation, provider submission, or hardware
execution.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31804184422 "SynQ Compiler Core #27"
[2]: https://github.com/TangoSplicer/SynQ/actions/runs/31804191932 "SynQ Compiler Core #28"
[3]: https://github.com/TangoSplicer/SynQ/actions/runs/31837377648 "SynQ Compiler Core #29"
[4]: https://github.com/TangoSplicer/SynQ/actions/runs/31842571512 "SynQ Compiler Core #30"
[5]: https://github.com/TangoSplicer/SynQ/actions/runs/32069791226 "SynQ Compiler Core identifier-if platform matrix"
