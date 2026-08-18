# Alpha Literal-Negation `if` OpenQASM 3 Lowering

**Status:** Design and implementation contract for one proposed Alpha
strict-Hybrid source-lowering extension. It is not implemented, locally
executed, provider-supported, or hardware-supported until focused tests and the
remote Compiler Core matrix have passed.

## Purpose and scope

The existing strict-Hybrid subsets export literal `if`, an identifier backed by
an earlier Boolean-literal declaration, and `not` over that identifier. This
increment adds **only** source-level compile-time folding for the two remaining
unary literal forms:

```synq
if not true then quantum h q[0]
if not false then quantum h q[0]
```

The output is deliberately the existing literal condition spelling, with no
generated Boolean declaration or storage:

```openqasm
if (false) h q[0];
if (true) h q[0];
```

OpenQASM defines `!` as a Boolean logical operator, supports Boolean literals,
and permits `if` conditions that evaluate to Boolean values.[1] [2] SynQ uses
that target-language fact only to justify this exact source equivalence. It does
not adopt OpenQASM's general runtime expression, assignment, branch, or loop
semantics.

## Exact acceptance and transformation rule

| Source condition | Required structural form | Strict-Hybrid output condition | Target storage |
| --- | --- | --- | --- |
| `true` | Existing literal condition | `true` | None |
| `false` | Existing literal condition | `false` | None |
| `not true` | `Not` with exactly one Boolean-literal operand | `false` | None |
| `not false` | `Not` with exactly one Boolean-literal operand | `true` | None |
| `not enabled` | Existing separately documented identifier-negation subset | `!synq_bool_enabled` | Existing immutable generated Boolean declaration only |

Every accepted form continues to require one Alpha-gated `if`, exactly one
supported typed quantum-gate body, declared in-range operands, and no `else`
or body block. The exporter must preserve its all-or-nothing behavior: any
unsupported node produces diagnostics and no target program text.

## Deliberate exclusions

| Rejected source family | Why it remains outside this increment |
| --- | --- |
| `not (not true)`, nested `not`, or parentheses | The parser's bounded tree representation exists, but recursive expression folding and parentheses semantics are not being specified here. |
| `ready and fallback`, `ready or fallback`, or other binary expressions | Operator evaluation, ordering, and target expression policy require a separate contract. |
| `not alias` | Alias identity and target-storage mapping remain separately undefined. |
| `not observed` after a measurement result | Measurement feedback storage and target mapping remain unimplemented. |
| `while`, `else`, blocks, measurement bodies, or callable bodies | Loop, branch-region, result, scope, and execution semantics remain undefined. |
| Simulator, C ABI, provider, or hardware behavior | This is OpenQASM source generation only and adds no runtime path. |

## Required evidence

Before a capability claim, implementation must provide exact-output positive
tests for both source literals; retain negative coverage for nesting, aliases,
binary expressions, measurement results, loops, and measurement bodies; emit
the fixture through `synqc`; validate the resulting OpenQASM with the reference
parser; and pass the complete local and five-job remote Compiler Core matrix.

## References

[1]: https://openqasm.com/language/classical.html "OpenQASM Classical Instructions — Boolean logical operators and `if` statements"

[2]: https://openqasm.com/language/types.html "OpenQASM Types and Casting — Boolean types and compile-time constants"
