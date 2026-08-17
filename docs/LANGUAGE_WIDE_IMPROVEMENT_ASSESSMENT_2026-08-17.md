# SynQ Language-Wide Improvement Assessment

**Review date:** 17 August 2026  
**Status:** Evidence-based improvement assessment for the experimental recovery
profile. This is not a release plan, feature-completeness claim, or statement of
production readiness.

## Executive conclusion

SynQ has a credible bounded compiler kernel: typed recovery nodes, structured
diagnostics, deterministic local probability simulation for a narrow default
register, strict OpenQASM source export, an opaque C ABI, and independently
scoped CI evidence. The most important work is now **semantic consolidation**,
not adding broad syntax or provider integrations. The current AST and Hybrid IR
preserve typed parsed forms but intentionally do not define general values,
scopes, ownership, resource lifetime, execution, or backend semantics.[1]

> The highest-leverage next milestone is an **alpha semantic kernel**: a written,
> testable contract for values, bindings, resources, effects, and lowering
> boundaries. It should precede multi-register simulation, non-literal control,
> callable bodies, and packaged language wrappers.

## Current strengths and limiting boundary

| Area | Current evidence-backed strength | Limit that prevents a broader language claim |
| --- | --- | --- |
| Syntax and diagnostics | Typed recovery nodes and diagnostic codes cover a constrained declaration, gate, measurement, expression, and callable-declaration surface. | The parser profile is intentionally narrow and is not a complete grammar or language specification. |
| Quantum source workflow | Strict OpenQASM 3 export and bounded ideal probability simulation have exact fixtures and rejection boundaries. | Export and simulation do not supply a general execution or hardware model. |
| Classical surface | Bounded literals, aliases, one-operator Integer expressions, and small Boolean expressions are represented and checked. | There are no general values, assignments, scopes, precedence model, or runtime value semantics. |
| Resources | Declared named registers resolve and strict export preserves their spelling. | Allocation, lifetime, aliasing, flattening, and multi-register simulation remain deliberately undefined. |
| Control | One literal `if` with one supported gate body lowers as source. | Identifier/expression conditions, loops, measurement bodies, and runtime branch behavior lack a classical state model. |
| Calls | `fn` and `kernel` declarations preserve metadata. | Parameters, bodies, calls, return values, recursion, effects, and call lowering/execution are absent. |
| Interoperability | Opaque C ABI consumers are remotely exercised; static SDK clean-install consumers now pass on Ubuntu and Windows. | There is no stable ABI, safe wrapper, registry package, or language-specific lifecycle contract. |

## Prioritized improvement sequence

### P0 — Specify the alpha semantic kernel

Before adding new surface syntax, publish one concise semantic contract that
answers the following questions for the bounded subset: which values exist;
which declarations introduce bindings; how duplicate, shadowed, and immutable
bindings behave; which expressions have types; which operations can fail; and
which AST-to-Hybrid-IR invariants are preserved. The recovery AST currently
contains bounded Boolean and Integer expression trees, declaration-only callables,
typed gates, measurements, and control nodes, but those types alone do not define
an execution model.[2]

The implementation increment should be limited to a written type-and-binding
table, a resolver-owned symbol/type environment, explicit diagnostics for every
unsupported value or effect, and focused positive/negative fixtures. This creates
the vocabulary every later feature needs without claiming a general runtime.

### P1 — Define named-register resources before simulating them

Named registers are the immediate quantum-semantics blocker. Their current Alpha
contract correctly validates declaration order and index range and supports strict
export, while explicitly rejecting the form in the simulator.[3] The next
contract must select one bounded representation: either multiple distinct
registers with stable identities, or a documented flattened physical layout. It
must state allocation order, zero-size rejection, operand aliasing rules,
measurement-result association, resource limits, and the exact simulator mapping.

Only after that contract is approved should SynQ add a small multi-register
simulation slice, with deterministic state ordering, resource-limit diagnostics,
and cross-checks against strict export. This is more valuable than adding many
new gates because it resolves the meaning of existing named syntax.

### P1 — Choose a classical-state model before non-literal control

The parser and IR can represent a constrained control node, but a non-literal
condition needs a source of values, boolean conversion rules, and a target or
local-execution meaning. The existing control record correctly requires exact
OpenQASM lowering evidence for the one literal-gate case and rejects broader
forms.[4]

SynQ should choose one path per feature: **target lowering** when a source subset
has an exact backend representation, or **local execution** only when bounded
classical state, measurement-result values, termination, branch effects, and
resource limits are all specified. A safe next increment is a non-literal `if`
over an immutable Boolean binding with one gate body, provided it has a defined
OpenQASM mapping and explicit rejection where no mapping exists. `while`, `else`,
blocks, and measurement-body control should remain out of scope until then.

### P1 — Introduce callable bodies only after scopes and resources

Callable declarations are useful names but are intentionally metadata-only.[5]
The first real callable subset should be non-recursive and effect-bounded: typed
parameters, no implicit capture, a declared quantum-resource parameter policy,
no dynamic dispatch, explicit return rules, and a static call graph. Its tests
should prove rejection of recursion, duplicate parameters, unknown calls, illegal
resource aliasing, and calls unsupported by the selected backend.

This work must follow the semantic kernel and named-register decision. Otherwise,
function calls would freeze unknown scope, value, and qubit ownership rules into
the public language surface.

### P2 — Simplify compiler architecture around the proven subset

The compiler should gradually move from the recovery AST’s broad legacy virtual
placeholder surface and manually owned node collections toward explicit typed
ownership, visitors, and a small set of documented IR invariants.[2] This is a
maintainability improvement, not a new user-facing feature. It lowers the cost of
adding source spans, type checks, resource effects, and backend diagnostics
without making speculative subsystems part of the supported build.

Each refactor should preserve current syntax, diagnostics, and test outputs, and
should ship only with regression fixtures. Avoid reviving unfinished debugger,
optimizer, network, REPL, plugin, or hardware subsystems until their own contracts
and evidence exist.

### P2 — Deliver one idiomatic interoperability proof

Once one semantic increment is complete, Rust is the lowest-risk first packaged
adapter because a Rust opaque-C-ABI smoke consumer already exists. The adapter
should expose ownership-safe handles and structured errors, pin a compatible
experimental ABI range, include a native-language conformance suite, and use a
distribution method maintainable by one developer. Common Lisp, Clojure, and
Mercury should remain consumer proofs until each has equivalent ownership,
packaging, and lifecycle evidence.[1]

### P3 — Defer providers, hardware, AI, and optimization claims

Quantum hardware access, provider credentials, AI assistance, noise modeling,
optimization, IDE services, and cloud execution are separate products, not
natural consequences of parser growth. Each would need an independent threat
model, consent and privacy contract, service boundary, cost model, test strategy,
and support matrix. They should not be used to imply that the language core is
more complete than it is.[1]

## Recommended next two implementation increments

| Increment | Concrete deliverable | Completion evidence | Explicit non-goals |
| --- | --- | --- | --- |
| **1. Alpha semantic kernel record** | A value/binding/effect design record; a resolver-owned type environment; exact diagnostics for unsupported uses. | Positive and negative compiler fixtures; updated typed-IR invariants; remote Compiler Core evidence. | General runtime, mutation, closures, provider access, stable ABI. |
| **2. Named-register resource contract** | Allocation and lifetime rules plus a bounded multi-register simulator mapping. | Deterministic simulator vectors, strict-export comparison fixtures, resource-limit tests, remote CI. | Hardware mapping, dynamic allocation, noise, unbounded registers, general type system. |

After these two increments, re-evaluate whether a non-literal `if` target-lowering
slice has enough semantic support. Do not begin callable bodies before that
re-evaluation.

## Platform and delivery implication

The current five-job CI matrix is a meaningful engineering improvement: Linux
full-profile coverage, Windows and macOS platform-neutral smoke coverage, and
experimental static SDK clean-install consumer checks on Ubuntu and Windows.[6]
It improves confidence in the bounded kernel and its build portability. It does
not change SynQ into a cross-platform package product, a stable SDK, or a fully
operational general-purpose language. macOS SDK conformance, wrapper lifecycle
contracts, signing/provenance, and release-version policy remain separate work.

## References

[1] [Operational readiness assessment](./OPERATIONAL_READINESS_ASSESSMENT_2026-08-15.md)  
[2] [Typed recovery AST](../compiler/src/compiler/ast.h) and [Hybrid IR](../compiler/src/compiler/hybrid_ir.h)  
[3] [Named-register operand contract](./NAMED_REGISTER_OPERANDS.md)  
[4] [Classical control-flow contract](./CLASSICAL_CONTROL_FLOW.md) and [strict literal-if lowering](./STRICT_IF_LOWERING.md)  
[5] [Callable declaration contract](./CALLABLE_DECLARATIONS.md)  
[6] [Compiler Core platform-matrix run #32018610062](https://github.com/TangoSplicer/SynQ/actions/runs/32018610062)
