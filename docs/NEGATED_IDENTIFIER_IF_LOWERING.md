# Alpha Negated Identifier-`if` OpenQASM 3 Lowering

**Status:** Design and implementation contract for one proposed Alpha strict-Hybrid
source-lowering extension. It must not be presented as implemented, locally
executed, provider-supported, or hardware-supported until its focused tests and
remote compiler matrix have passed.

## Scope

The existing identifier-`if` contract maps an earlier SynQ Boolean-literal
declaration to immutable generated OpenQASM Boolean storage. This extension
allows exactly one unary source form over that same proven storage mapping:

```synq
#[experimental(feature = "qubit-declarations")]
#[experimental(feature = "classical-control-flow")]
let enabled = true
qubit q[1]
if not enabled then quantum h q[0]
```

The proposed strict-Hybrid output is:

```openqasm
OPENQASM 3.0;
include "stdgates.inc";
qubit[1] q;
bool synq_bool_enabled = true;
if (!synq_bool_enabled) h q[0];
```

OpenQASM Boolean values support the unary logical `!` operator, and an `if`
condition is a Boolean expression.[1] OpenQASM also permits classical values to
be initialized when declared; SynQ emits one initialized Boolean declaration and
no later assignment.[2]

## Exact acceptance rule

| Item | Required condition |
| --- | --- |
| SynQ condition | A parsed `ClassicalBooleanExpressionKind::Not` with exactly one `IdentifierReference` operand. |
| Referenced source binding | An earlier top-level `let name = true` or `let name = false` declaration already accepted by the identifier-`if` contract. |
| Control form | `if`, never `while`. |
| Body | Exactly one existing supported typed quantum gate with earlier declared, in-range operands. |
| Target storage | Reuse the existing `bool synq_bool_<name> = true|false;` mapping; emit no assignment, temporary, or extra storage. |
| Target condition | Emit exactly `!synq_bool_<name>` inside the existing `if (...)` syntax. |
| Feature boundary | Require the existing Alpha `classical-control-flow` opt-in. No new feature gate broadens the parser surface. |

## Deliberate exclusions

| Rejected form | Reason |
| --- | --- |
| `if not true` or `if not false` | This increment proves negation only over declared generated storage, not expression folding. |
| `if not alias` where `let alias = enabled` | Alias propagation and target-storage identity remain a separate contract. |
| `if not observed` after a named measurement result | Measurement-result export and feedback storage are still not implemented. |
| `if not ready and fallback`, `if ready or fallback`, nesting, or parentheses | Binary-expression evaluation, precedence, and temporary values remain outside the strict source subset. |
| `while not enabled`, `else`, blocks, or measurement bodies | Loop, branch-region, termination, and conditional-measurement semantics are not defined. |
| Local simulation or ABI execution | This is target text generation only; it does not add a runtime or branch-execution model. |

## Required evidence before a capability claim

The implementation must add exact-output positive coverage, retain focused
negative tests for each excluded family, generate the source through `synqc`,
and validate the generated text with the OpenQASM 3 reference parser. It must
then pass the full local recovery profile and all five remote Compiler Core jobs.
The earlier identifier-`if` evidence must remain intact.

## References

[1]: https://openqasm.com/language/classical.html "OpenQASM Classical Instructions — Boolean operators and if statements"

[2]: https://openqasm.com/language/types.html "OpenQASM Types and Casting — classical variable initialization and Boolean types"
