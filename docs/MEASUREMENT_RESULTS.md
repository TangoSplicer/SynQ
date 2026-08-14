# Bounded Measurement-Result Declarations

**Status:** Design and implementation in progress; no remote validation claim is
made in this record.
**Last reviewed:** 14 August 2026

## Purpose

This recovery-profile increment permits a top-level measurement to declare a
named static Boolean binding for later bounded classical-control validation. It
creates source, Hybrid IR, and resolver provenance only. It does not measure a
qubit, produce a runtime value, or execute a branch.

> **Design rule:** `as <identifier>` records a future-compatible result binding,
> not an observed bit. The name is statically typed as Boolean solely for the
> existing bounded Boolean condition profile.

## Accepted grammar

```synq
measure q[0] as observed
```

The existing `measure q[index]` form remains valid and declares no result name.
The result identifier is optional but, when present, must satisfy the ordinary
identifier rule and shares the parser's top-level uniqueness namespace. Invalid
forms use `SYNQ-P008`; duplicate result names use `SYNQ-S004`.

## Internal behavior and export boundary

`MeasurementNode` and `HybridMeasurement` retain an optional result name. The
internal resolver records that name as a Boolean binding at the measurement
statement’s program index, so a later bounded `if observed then ...` may resolve
it. A forward condition still returns `SYNQ-R002`.

The bounded OpenQASM 3 exporter explicitly rejects named SynQ measurement-result
declarations rather than silently dropping the result-binding semantics. Plain
unnamed measurements retain their existing OpenQASM lowering.

## Explicit non-goals

This increment does not perform a measurement, expose observed bits, define
measurement probability, provide runtime values, define assignment or mutation,
allow result-name use outside the existing bounded Boolean-condition profile,
support result names in control bodies, lower named results to OpenQASM,
simulate, execute, submit to providers, or access hardware.

## Focused validation

`synq_measurement_result_smoke` covers typed parser and Hybrid IR provenance,
static Boolean binding for a later bounded condition, malformed and duplicate
names, forward-use rejection, legacy unnamed measurement compatibility, and
explicit OpenQASM-export rejection for named results. The local recovery profile
reported **21/21** CTest checks. This is local evidence pending publication and
compiler-core CI.
