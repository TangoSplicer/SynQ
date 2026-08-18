# Alpha Bounded Callable Kernels and Calls

**Status:** Design contract only. It does not describe an implemented SynQ
capability until focused local tests and all five remote Compiler Core jobs have
passed.

## Chosen first subset

The first callable increment is intentionally narrower than general functions or
OpenQASM subroutines. It adds only a non-recursive **kernel** declaration with
one already-declared global qubit operand in a one-gate body, plus a later
top-level invocation:

```synq
#[experimental(feature = "callable-declarations")]
#[experimental(feature = "qubit-declarations")]
qubit q[1]
kernel prepare() { quantum h q[0] }
call prepare()
```

Strict Hybrid OpenQASM lowering expands the call at compile time into the
already supported body gate:

```openqasm
qubit[1] q;
h q[0];
```

The call itself has no runtime dispatch, stack frame, value, return, or local
execution meaning. Its exact resource effect is the statically checked gate
operand recorded in the declared kernel body.

This mirrors only the safest relevant OpenQASM principles: a callable signature
must be known at compile time, qubits cannot be declared inside subroutine
bodies, and source definitions must precede use.[1] [2] SynQ does **not** adopt
OpenQASM's general parameters, local scopes, return values, recursion, or
subroutine execution behavior.

## Acceptance rules

| Element | Required form | Explicit behavior |
| --- | --- | --- |
| Declaration | `kernel <identifier>() { quantum <supported gate> <earlier declared register[index]> }` on one source line | Exactly one body gate; parser stores the typed body and declaration provenance. |
| Parameters | Empty parentheses only | No quantum/classical parameters, capture, alias, or argument binding. |
| Body resource | Existing named/default register operand with literal in-range index | The declaration may only use a register already declared earlier at top level. |
| Call | `call <identifier>()` after the kernel declaration | One call name resolves to one earlier kernel; strict Hybrid export expands its one typed gate in source order. |
| Kind | `kernel` only | Existing `fn name()` remains declaration-only and has no body/call behavior. |
| Feature gate | Existing `callable-declarations` Alpha opt-in | No implicit activation or un-gated source form. |

## Non-goals and rejections

The first subset rejects function bodies, parameters, return values, multiple
body statements, calls before declarations, self or mutual recursion, calls from
control bodies, nested calls, local declarations, qubit declarations in a body,
measurements, conditionals, loops, gate parameters, dynamic operands, and
simulation or C-ABI execution. A call is permitted only at top level and only
expands during strict Hybrid OpenQASM source generation.

## Required proof

Implementation must add parser, typed-AST, Hybrid-IR/resolution, duplicate and
unknown-call diagnostics, exact strict-Hybrid output, CLI emission, and OpenQASM
reference-parser fixtures. Full local recovery and the five-job remote matrix
must pass before documentation describes the subset as verified.

## References

[1]: https://openqasm.com/language/subroutines.html "OpenQASM Subroutines — compile-time signatures and qubit declarations"

[2]: https://openqasm.com/language/scope.html "OpenQASM Scoping of Variables — definition-before-use and subroutine scope"
