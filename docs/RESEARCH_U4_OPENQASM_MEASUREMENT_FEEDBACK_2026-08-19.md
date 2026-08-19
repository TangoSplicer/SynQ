# U4 Measurement-Feedback Research Notes

**Purpose:** External design input for a proposed bounded SynQ U4
measurement-feedback contract. This note describes OpenQASM source material; it
does not itself claim implemented SynQ behavior.

## Primary findings

The OpenQASM language documentation distinguishes low-level classical
instructions embedded in a quantum circuit from wider external classical
functions. It specifies that assignments require matching left- and right-hand
side types, and that `if (bool)` may guard one statement or a block.[1]

OpenQASM’s type documentation describes scalar `bit` as taking values `0` or
`1`, `bit[n]` as a static classical register, and an r-value scalar `bit` as
interchangeable with `bool`; a bit-valued expression may therefore be used as
an `if` condition.[2] The same documentation identifies classical registers as
controller state exposed within the program.[2]

These capabilities are broader than the appropriate SynQ U4 target. The
OpenQASM material also permits assignments, comparisons, Boolean expressions,
multi-statement branches, loops, and target-dependent runtime features. SynQ
must not inherit any of those semantics implicitly.[1] [2]

## Constrained U4 implications

The smallest defensible SynQ slice is a typed named measurement result followed
by one later correction gate conditioned on that exact result. A candidate
source shape is `measure q[0] as observed` followed by `if observed then quantum
x q[1]`. The local representation must distinguish a result declaration from a
runtime sample: preserving a typed result reference does not imply simulated
collapse or an observed value.

Strict Hybrid OpenQASM lowering could introduce one target `bit` declaration,
assign the measurement to that bit, and emit one `if (bit)` gate body. This is
source generation only. U4 should reject `else`, loops, Boolean expressions,
aliases, result reassignment, reuse after another measurement, target-side
state, classical evaluation, local feedback execution, ABI execution, hardware
submission, and provider behavior unless each has a later dedicated contract.

## References

[1] [OpenQASM live specification: Classical instructions](https://openqasm.com/language/classical.html)

[2] [OpenQASM live specification: Types and Casting](https://openqasm.com/language/types.html)
