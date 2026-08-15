# Recovery-Profile Performance Evidence

**Status:** The benchmark implementation is included in the remotely validated
recovery compiler build; its timing observation remains local-only evidence.
**Last reviewed:** 15 August 2026

## Measurement scope

SynQ currently has one opt-in deterministic local benchmark:
`synq_benchmark`. It repeatedly prepares the same two-qubit Bell-state
probability model through the bounded simulator after parsing, lowering, and
resolution have already completed. Its output includes iteration count, elapsed
seconds, simulations per second, and a checksum to discourage dead-code removal.

```bash
cmake -S compiler -B compiler/benchmark-build \
  -DCMAKE_BUILD_TYPE=Release -DBUILD_RECOVERY_BENCHMARKS=ON
cmake --build compiler/benchmark-build --target synq_benchmark --parallel 2
./compiler/benchmark-build/synq_benchmark 1000
```

The benchmark is **not** registered as a CTest performance gate because timing
varies by processor, operating-system scheduling, compiler, and thermal state.
It is a reproducible local measurement tool, not a claim that SynQ is as fast as
Python, Java, or any other implementation.

## Interpretation boundary

| Measured | Not measured or claimed |
| --- | --- |
| Repeated bounded two-qubit local probability calculation after front-end preparation | General language throughput, compile-time performance, memory use, large circuits, noise, provider latency, hardware throughput, or cross-language comparison |
| Correctness checksum for the fixed benchmark fixture | A competitive benchmark suite, regression threshold, or performance SLA |

Any future language-performance claim requires published workloads, command
lines, machine/compiler details, repeated measurements, and an appropriate
comparison baseline.

## Local observation

On 15 August 2026, the documented Release benchmark command completed 1,000
bounded Bell-probability simulations in **0.000118557 seconds**, reporting
approximately **8.43 million simulations per second** and checksum `500` in the
current sandbox. The codebase containing the opt-in benchmark also passed
**26/26** recovery-profile checks in [Compiler Core #45](https://github.com/TangoSplicer/SynQ/actions/runs/31887461976) for revision `202ebaf`; the workflow does not
run a timing threshold or establish a remote performance measurement. This is
one local observation for the fixed two-qubit fixture; it is not a benchmark
result for the SynQ language as a whole and must not be used as a Python, Java,
hardware, or provider comparison.
