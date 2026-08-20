# Deterministic Parser/Runtime Replay Contract

**Status:** Design contract for the next reliability increment; implementation
evidence is pending.  
**Issued:** 20 August 2026.  
**Scope:** The C++17 recovery parser, Hybrid IR lowering, bounded name resolver,
and bounded local evaluator entry points.

## Purpose

This contract defines a zero-dependency, deterministic property-and-replay
target for the experimental SynQ recovery profile. It is intentionally **not** a
claim of libFuzzer integration, continuous fuzzing infrastructure, security
certification, complete parser coverage, general runtime testing, or hardware
validation.

The target must exercise parser input handling and the bounded runtime pipeline
without network access, a Python runtime, external SDKs, device access, temp
files generated from random names, or wall-clock behavior. SynQ corpus programs
remain written in SynQ; C++ is used only for the compiler test harness.

## Exact pipeline

For every corpus source and every deterministic mutation, the harness shall run
the following stages in process:

1. `Parser::parseSourceWithDiagnostics()`;
2. `lower_to_hybrid_ir()` only if parsing succeeds;
3. `resolve_hybrid_names()` only if lowering succeeds;
4. `evaluate_bounded_constants()`, `evaluate_bounded_state()`, and
   `evaluate_bounded_runtime()` only if resolution succeeds, each with the
   explicit local opt-in and its existing default resource limits.

Each stage must return a finite terminal outcome: an accepted value or a
structured diagnostic. No parser or runtime error may be ignored. The harness
does not invoke OpenQASM export, simulation, ABI calls, external parser/importer
checks, providers, or hardware.

## Corpus contract v0.1.0

The checked-in corpus directory is `compiler/tests/fuzz/corpus/`. The manifest
is a fixed ordered list of small, NUL-free UTF-8 SynQ source files. It contains
at least one accepted bounded runtime program, one accepted quantum/control
program, one accepted mutable-state program, one accepted measurement-feedback
program, and malformed or disabled-feature sources that must terminate in a
structured parser or semantic diagnostic.

| Corpus rule | Required behavior |
| --- | --- |
| File names and order | Checked in and treated as replay inputs, not generated test artifacts. |
| Source language | SynQ syntax only. No Python/Qiskit/Cirq/Q# program is a corpus input. |
| Input size | The harness rejects a corpus file over 4 KiB before handing it to the parser. |
| Text transport | Mutators preserve NUL-free byte strings; they never introduce a NUL byte. |
| Success semantics | A source may be accepted or rejected. A rejection is a successful harness outcome when it is deterministic and structured. |
| Versioning | Any intentional corpus or schedule change must update this contract, the manifest, focused tests, changelog, and evidence records. |

## Deterministic mutation schedule

Each corpus entry is replayed unmodified, then replayed with a fixed sequence of
eight `uint32` seeds. A small in-process xorshift generator chooses a bounded
single-byte replacement, insertion, or deletion; the output remains below 4 KiB
and contains no NUL. The schedule is fixed by source order and seed values,
not by time, host entropy, filesystem enumeration, or test parallelism.

The invariant is **same input plus same seed equals the same outcome digest**
within the same executable invocation. The digest must include the terminal
stage, acceptance flag, structured diagnostic codes/spans, and any bounded
evaluator binding values. It must not include pointers, process IDs, timestamps,
addresses, file-system paths, or unordered-container iteration results.

This property catches accidental state retention and nondeterministic diagnostic
or evaluator behavior. It does not prove that all malformed inputs are safe,
that every behavior is stable across compiler versions, or that a changed digest
is a bug without a human review of the corresponding language contract.

## Reproduction interface

The test executable accepts a corpus directory and optional exact replay
selectors. Its required interface is:

```bash
./compiler/build/synq_deterministic_replay_smoke \
  compiler/tests/fuzz/corpus \
  --case u6_binary_add.synq --seed 0x9e3779b9
```

With no selector, it runs the complete corpus and fixed schedule. With a case
and seed, it runs only that exact base source and mutation schedule member. It
prints the source name, seed label, terminal stage, and deterministic digest so
a failure log can be replayed locally. The CTest owner is
`synq_deterministic_replay_smoke`.

## Resource and failure rules

| Concern | Contract |
| --- | --- |
| Harness memory | Corpus input is capped at 4 KiB; mutation output is capped at the same bound; no unbounded corpus discovery is permitted. |
| Runtime work | Existing evaluator declaration, invocation, depth, and operation bounds remain authoritative. The harness must not widen them. |
| Atomic failures | If an evaluator rejects a source, its result must remain absent and its diagnostics become part of the replay digest. |
| Corpus errors | Missing manifest file, duplicate manifest entry, unknown selected case, unreadable file, oversize input, malformed seed syntax, or invalid selector combination fails the harness deterministically. |
| Platform behavior | The target is ordinary C++17 CTest coverage. It has no platform-specific dependency and must remain suitable for the existing Linux/Windows/macOS platform-neutral profile. |

## Evidence promotion rule

This contract may be described as implemented only after the focused target,
the full ordinary local recovery profile, and the required remote Compiler Core
matrix pass for one revision. The sanitizer profile must also include the target
or explicitly state why it is excluded. Until then, the known-limit ledger keeps
fuzzing and replay unclaimed.
