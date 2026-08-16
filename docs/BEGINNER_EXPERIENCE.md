# SynQ Beginner Experience Contract

**Status:** Design record for the public Evidence Ledger guided walkthrough and
visual Bell-state lesson. This record does not add a hosted compiler, simulator,
or quantum service.

## Purpose

The first-learning path should reduce setup and vocabulary friction without
misrepresenting SynQ’s current architecture. A visitor must be able to understand
the documented local workflow, copy exact commands, inspect a small circuit, and
see the expected probability meaning before installing the compiler.

## Guided CLI walkthrough

The public walkthrough is a **non-executing command guide**. It presents three
repository-backed example steps: validate a Bell-state source file, emit strict
Hybrid OpenQASM source, and request bounded local probabilities. A step may be
selected and its command copied to the browser clipboard. It must visibly state
that it does not run code, submit a job, connect to hardware, or return live
compiler output.

Expected output shown in the guide is fixture-style explanatory text, not an
online execution transcript. The commands remain useful only after the visitor
has built `synqc` locally through the tested-environment instructions.

## Visual Bell-state lesson

The lesson visualizes the documented two-qubit sequence `h q[0]` then
`cx q[0], q[1]`. It explains the initial state, the superposition step, and the
correlated probability outcome. The displayed probability split, 50% `|00⟩` and
50% `|11⟩`, is an exact ideal-state explanation for that circuit; it is not a
sample, hardware measurement, noise model, or claim of general circuit
simulation.

## Accessibility and interaction rules

The walkthrough uses semantic buttons, keyboard-reachable steps, visible selected
state, and text labels in addition to circuit symbols. The copy action reports
success or an instruction to select the command manually if browser clipboard
access is unavailable. Motion is optional and respects reduced-motion settings.

## Non-goals

This increment does not accept user source code, execute commands in the browser,
call a backend, store data, create accounts, provide a quantum simulator API, or
promise that every code example compiles under future experimental revisions.
