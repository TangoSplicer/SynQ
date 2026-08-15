# Bounded Constant Evaluation

**Status:** Design and implementation in progress; no remote validation claim is
made in this record.
**Last reviewed:** 15 August 2026

## Purpose

This is SynQ’s first deterministic executable semantic slice. It evaluates only
a resolved sequence of supported top-level declarations. It is deliberately not
a general runtime, interpreter, simulator, or quantum executor.

## Explicit opt-in and limits

The C++ API requires `allow_experimental_constant_evaluation = true`; its default
is false. The evaluator also has a caller-supplied declaration limit, defaulting
to 64. Exceeding the limit fails with `SYNQ-E001` before an output is produced.

## Supported values and source forms

| Form | Result |
| --- | --- |
| Integer literal | Signed `int64` value |
| Boolean literal | `true` or `false` value |
| Quoted string literal | Unescaped source contents between the parser-validated quotes |
| Earlier declaration identifier | Alias of the earlier evaluated supported value |
| Alpha bounded Integer arithmetic | One parser-produced `+`, `-`, or `*` tree over Integer literals/earlier Integer references |

Integer arithmetic uses checked `int64` arithmetic. Overflow, malformed internal
trees, missing prior values, and unsupported initializer forms fail with typed
`SYNQ-E003` through `SYNQ-E005` diagnostics. Decimals are intentionally rejected
until decimal precision, rounding, and error semantics are separately designed.

## Explicit exclusions

The evaluator rejects quantum gates, qubit declarations, measurements,
measurement-result values, controls, callables, decimals, opaque source,
assignment, mutation, loops, callable calls, I/O, network access, plugins,
parallel execution, simulation, provider integration, and hardware submission.

> A successful constant-evaluation result proves only deterministic evaluation
> of the documented declaration subset. It does not mean a SynQ program has run
> on a quantum simulator or device.
