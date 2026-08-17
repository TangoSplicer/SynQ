# Strict Literal `if` Lowering

**Status:** Remotely validated strict-export increment in [Compiler Core
#48](https://github.com/TangoSplicer/SynQ/actions/runs/31952214849). This is
source lowering evidence, not runtime control-flow execution evidence. The
separate later identifier-`if` contract is documented in
[`IDENTIFIER_IF_LOWERING.md`](./IDENTIFIER_IF_LOWERING.md).

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
| Earlier Boolean-literal declaration identifier | Lower under the separate identifier-`if` contract | The later bounded slice emits immutable generated target Boolean storage and one gate body only. |
| Identifier alias, `not`, `and`, or `or` condition | Reject | No declaration-alias or Boolean-expression evaluation/lowering contract exists. |
| `while` | Reject | No bounded loop execution or termination semantics are defined. |
| Named measurement result | Reject | Existing strict exporter limitation remains unchanged. |
| Unsupported gate, undeclared register, or out-of-range operand | Reject | Existing strict-export safety checks remain mandatory. |
| Local simulation | Reject program shape | The bounded simulator remains a no-control-flow subset. |

The `if false` form is exported rather than constant-folded so source provenance
and the explicit control structure remain inspectable. This is source lowering,
not a statement that a SynQ runtime executes branches.

## Non-goals

The literal increment itself does not define classical variables in OpenQASM,
measurement-dependent branching, loops, scope, branch blocks, multi-statement
regions, callable bodies, general expression evaluation, optimization, provider
access, or hardware execution. The later identifier-`if` exception is limited to
an immutable Boolean literal declaration and does not alter those wider
non-goals. Any broader addition requires a separate design record, negative
tests, and new remote validation evidence.

## Evidence

Revision `8f0de7e` passed **27/27** recovery-profile CTest checks in [Compiler
Core #48](https://github.com/TangoSplicer/SynQ/actions/runs/31952214849). The
coverage asserted exact literal-if OpenQASM text, separate-process CLI output,
and, at that time, rejection of identifier conditions, `while`, and measurement
bodies. The later bounded identifier-`if` exception is independently documented
and validated; no external-provider, hardware, general-classical, or
local-simulator control claim follows from either source-generation record.
