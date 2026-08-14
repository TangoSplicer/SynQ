# Bounded Callable Declarations

**Status:** Design and implementation in progress; no remote validation claim is
made in this record.
**Last reviewed:** 14 August 2026

## Purpose

This recovery-profile increment introduces typed provenance for declaration-only
classical functions and quantum kernels. It creates no callable behavior. The
goal is to establish a safe shape for later work without misleading users into
thinking that a function body, call, or runtime exists.

> **Design rule:** a callable declaration is metadata, not behavior. `fn` and
> `kernel` declare a name and kind only.

## Alpha grammar

After `#[experimental(feature = "callable-declarations")]`, the accepted forms
are exactly:

```synq
fn prepare()
kernel ansatz()
```

Names use the recovery parser's existing identifier rule. Parameters, return
types, generic arguments, annotations, bodies, braces, calls, and nested
constructs are rejected with `SYNQ-P013`. Callables share the top-level
declaration namespace, so duplicate names use `SYNQ-S004`.

## Internal behavior

`CallableDeclarationNode` and `HybridCallableDeclaration` retain the callable
kind, name, and source provenance. The resolver preserves that metadata without
binding it as a value or interpreting it as a scope. The strict Hybrid OpenQASM
exporter rejects callable nodes rather than dropping them.

## Explicit non-goals

This increment does not provide parameters, function bodies, kernel bodies,
calls, return values, lexical scopes, overloads, generics, closures, recursion,
classical execution, quantum execution, control-flow execution, OpenQASM
lowering, simulation, provider integration, or hardware submission.

## Focused validation

`synq_callable_declaration_smoke` covers Alpha gating, typed `fn` and `kernel`
AST/Hybrid IR provenance, resolver preservation, malformed parameters/bodies,
shared top-level uniqueness, and strict Hybrid OpenQASM rejection. The local
recovery profile reported **23/23** CTest checks. This is local evidence pending
publication and compiler-core CI.
