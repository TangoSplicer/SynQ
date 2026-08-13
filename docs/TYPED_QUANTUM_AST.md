# SynQ Typed Quantum AST Boundary

**Status:** Locally verified recovery-profile implementation boundary; remote CI
evidence for this increment is pending publication.  
**Last reviewed:** 13 August 2026

## Purpose

The recovery parser previously represented quantum statements as generic
`InstructionNode` strings. That was adequate for a bounded parser/exporter
recovery profile, but it forced every later consumer to re-parse gate names,
parameter text, and qubit operands. This increment implements the first typed
quantum AST boundary without pretending that SynQ already has a full type
system or Hybrid IR.

> **Design rule:** construct typed quantum nodes immediately after the existing
> bounded syntax validation. Preserve the original gate spelling for diagnostics
> and unsupported constructs; do not infer semantics for arbitrary identifiers.

## Minimal vocabulary

| Element | Proposed representation | Responsibility |
| --- | --- | --- |
| `QuantumGateKind` | Enumeration: `H`, `X`, `Y`, `Z`, `Cx`, `BellPair`, `Rx`, `Ry`, `Rz`, `Phase`, `Unknown`. | Identifies only the subset whose gate shape is known in the recovery profile. |
| `QuantumGateNode` | `kind`, `source_name`, optional `literal_angle`, `qubit_indices`, `line`. | Represents one parsed `quantum` statement after syntax has already constrained operand form. |
| `source_name` | Original unparameterized gate identifier such as `rx` or `qaoa`. | Keeps unsupported or future gates diagnosable without inventing a semantic mapping. |
| `literal_angle` | Optional source-preserved text such as `pi/2`, `-pi/4`, or `0.125`. | Carries the already bounded literal-angle syntax without claiming expression typing. |
| `qubit_indices` | Ordered `std::size_t` indices. | Removes the need for the OpenQASM backend to re-parse `q[index]` strings. |

`Unknown` is intentional. The parser now recognizes a syntactically valid gate
identifier without declaring it executable, exportable, stable, or semantically
understood. The bounded OpenQASM backend must reject `Unknown` with its
preserved source name rather than guessing a lower-level representation.

## Parser and compatibility policy

The parser continues accepting the same recovery-profile source grammar. Its
successful `quantum` statements now produce `QuantumGateNode` instances;
declarations and non-quantum instructions remain their current nodes. The
legacy `InstructionNode` stays temporarily because existing internal fixtures
and historical source constructors use it. It is not promoted to a stable AST
API.

| Input shape | Parsed node | Export behavior |
| --- | --- | --- |
| `quantum h q[3]` | `QuantumGateNode{H, "h", null, {3}}` | Emits `h q[3];`. |
| `quantum cx q[3], q[5]` | `QuantumGateNode{Cx, "cx", null, {3, 5}}` | Emits `cx q[3], q[5];`. |
| `quantum rx(pi/2) q[0]` with alpha opt-in | `QuantumGateNode{Rx, "rx", "pi/2", {0}}` | Emits `rx(pi/2) q[0];`. |
| `quantum qaoa q[0]` | `QuantumGateNode{Unknown, "qaoa", null, {0}}` | Produces an explicit exporter diagnostic. |
| Direct legacy `InstructionNode("quantum", ...)` fixture | A temporary exporter adapter reconstructs the same typed shape. | Keeps existing fixture construction valid during the migration. |

No general symbolic angle, dynamic indexing, custom gate definition, qubit
allocation, measurement result, or runtime resource ownership is introduced by
this node. Those require a later semantic/type layer and a real Hybrid IR.

## Backend contract

OpenQASM lowering now consumes `QuantumGateNode` directly. A small legacy
adapter remains only inside the exporter to preserve direct historical fixture
construction while parser output has migrated. Gate arity remains a backend rule:
single-qubit gates accept no or one explicit operand under the recovery
contract; `Cx` needs exactly two; parameterized gates need one; and `BellPair`
accepts zero or two. The typed node does not itself claim that an operation is
valid for all backends.

## Exit criteria

The parser now constructs typed nodes; parser fixtures assert their fields and
exact output; unsupported typed nodes remain diagnosable; legacy direct
instruction fixtures continue to behave as documented; and compiler, frontend,
and backend validation pass locally. The status record identifies this as a
typed quantum **AST boundary**, not a complete type system or Hybrid IR. Remote
compiler-core evidence is required before the implementation is described as
remotely validated.
