# Bounded Local Quantum Simulation

**Status:** Remotely validated experimental local simulation slice in [Compiler
Core #43][1].
**Last reviewed:** 15 August 2026

## Purpose

This experimental subsystem calculates deterministic probabilities for a small
typed pure-state circuit subset. It is a local state-vector simulator, not a
quantum provider connector, hardware runtime, or replacement for device-specific
validation.

## Explicit opt-in and limits

The C++ API defaults to disabled and requires
`allow_experimental_local_simulation = true`. `synqc --simulate` makes that
opt-in explicit at the command line. Default limits are **10 qubits** and
**1,024 gates**, producing at most 1,024 complex amplitudes. Exceeding a limit
fails before output is produced.

## Supported circuit shape

The program must contain exactly one enabled `qubit q[n]` declaration, followed
by supported typed gates and then optional unnamed trailing measurements. The
supported gates are `h`, `x`, `y`, `z`, `cx`, `bell_pair`, `rx`, `ry`, `rz`, and
`p` with the parser’s documented literal-angle forms.

Measurements report each requested qubit’s **marginal probability of one**. They
do not sample a shot, collapse the state, bind a result, or affect later gates.
The basis display uses the conventional high-index-to-low-index bit order, so
two-qubit Bell preparation reports `|00>` and `|11>` with probability `0.5`.

## Rejection and safety boundaries

The simulator rejects missing/non-default/multiple registers, named measurement
results, gates after a measurement, unsupported gates, invalid internal gate
shape, controls, declarations, callables, classical evaluation, loops, calls,
I/O, plugins, provider access, and hardware submission. It also verifies final
state normalization before returning probabilities.

> A successful simulation is evidence only for this bounded local numerical
> model. It is not evidence of device behavior, noise modeling, measurement
> sampling, hardware compatibility, or provider execution.

## Focused validation

`synq_bounded_simulator_smoke` verifies Bell-state basis and marginal
probabilities, parameterized rotation probabilities, explicit opt-in refusal,
the qubit limit, and named-result rejection. The expanded separate-process CLI
smoke verifies deterministic `synqc --simulate` output. The local recovery
profile reported **26/26** CTest checks. This is local evidence pending
publication and compiler-core CI.

The local recovery profile and [Compiler Core #43][1] both reported **26/26**
CTest checks.

## References

[1]: https://github.com/TangoSplicer/SynQ/actions/runs/31886881473 "SynQ Compiler Core #43"
