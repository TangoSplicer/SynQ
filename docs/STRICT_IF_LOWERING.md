# Strict Literal `if` Lowering

**Status:** Design record for the next Alpha control-flow increment. This is not
evidence that control-flow export or execution is currently available.

## Narrow target

The parser and Hybrid IR already preserve an Alpha control node with `if` or
`while`, a bounded Boolean condition, and one typed quantum-gate or measurement
body. That representation has deliberately remained unsupported by the strict
Hybrid OpenQASM 3 exporter because it does not define general classical execution
or runtime state.

The first safe lowering target is only this form:

```synq
#[experimental(feature = "classical-control-flow")]
#[experimental(feature = "qubit-declarations")]
qubit q[1]
if true then quantum h q[0]
```

It lowers to a single OpenQASM 3 conditional statement with the already validated
typed body:

```openqasm
if (true) h q[0];
```

## Acceptance boundary

| Source/IR condition | Strict exporter action | Reason |
| --- | --- | --- |
| `if true` or `if false` plus one supported typed gate | Lower | The condition has a fixed source literal and no undeclared classical storage. |
| `if true` or `if false` plus one measurement | Reject | Conditional measurement assignment is intentionally deferred until an explicit target-side storage/control contract is tested. |
| Identifier, `not`, `and`, or `or` condition | Reject | No OpenQASM declaration/lowering contract exists for SynQ’s classical bindings or expression tree. |
| `while` | Reject | No bounded loop execution or termination semantics are defined. |
| Named measurement result | Reject | Existing strict exporter limitation remains unchanged. |
| Unsupported gate, undeclared register, or out-of-range operand | Reject | Existing strict-export safety checks remain mandatory. |
| Local simulation | Reject program shape | The bounded simulator remains a no-control-flow subset. |

The `if false` form is exported rather than constant-folded so source provenance
and the explicit control structure remain inspectable. This is source lowering,
not a statement that a SynQ runtime executes branches.

## Non-goals

This increment does not define classical variables in OpenQASM, measurement
dependent branching, loops, scope, branch blocks, multi-statement regions,
callable bodies, general expression evaluation, optimization, provider access,
or hardware execution. Any of those additions require a separate design record,
negative tests, and new remote validation evidence.
