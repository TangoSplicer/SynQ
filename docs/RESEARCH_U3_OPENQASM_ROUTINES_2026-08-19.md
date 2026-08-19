# U3 Routine-Design Research Notes

**Purpose:** Source record for the proposed SynQ U3 bounded quantum-routine
contract. This is research input, not a statement of implemented SynQ behavior.

## Relevant external constraints

OpenQASM specifies that subroutine parameters can include quantum and classical
arguments, that quantum bits are passed by reference or name, and that a given
underlying qubit may appear at most once in one subroutine call. It also bars
qubit declarations inside subroutine bodies.[1]

OpenQASM scope rules require symbols to be defined before use, prohibit forward
declarations and mutual recursion, and keep mutable global variables out of
subroutine/gate scope. They separately confirm that subroutines cannot declare
qubits in their bodies but may accept qubit parameters.[2]

OpenQASM gate definitions distinguish optional angle variables from required
qubit arguments; angle parameters appear before quantum arguments. Gate bodies
may contain built-in gates and calls to previously defined gates, while the
formal qubit arguments are identifiers rather than indexed expressions.[3]

## SynQ U3 design implications

The next SynQ slice should be deliberately smaller than OpenQASM. It should use
one explicit scalar angle formal and one or two explicit single-qubit formals,
with no local qubit declaration, capture, registers-as-parameters, aliases,
arrays, recursion, nested calls, returns, dynamic angle expressions, control
flow, measurement, simulation execution, or ABI execution. At each call site,
the compiler should require earlier caller-owned literal register/index operands
and reject duplicate physical qubit operands before lowering.

The initial target behavior should remain static source expansion of one typed
body gate, not runtime dispatch. A non-recursive definition-before-use graph and
parameter substitution must be resolved before strict Hybrid OpenQASM emission.

## References

[1] [OpenQASM Live Specification — Subroutines](https://openqasm.com/language/subroutines.html)

[2] [OpenQASM Live Specification — Scoping of Variables](https://openqasm.com/language/scope.html)

[3] [OpenQASM 3.0 Specification — Gates](https://openqasm.com/versions/3.0/language/gates.html)
