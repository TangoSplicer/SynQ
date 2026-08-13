#!/usr/bin/env python3
"""Import a generated OpenQASM 3 fixture into a Qiskit QuantumCircuit."""

from pathlib import Path
import sys


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: validate_qiskit_qasm3_import.py <generated-openqasm3-file>", file=sys.stderr)
        return 2

    fixture = Path(sys.argv[1])
    if not fixture.is_file():
        print(f"missing generated OpenQASM fixture: {fixture}", file=sys.stderr)
        return 2

    try:
        from qiskit_qasm3_import import parse
    except ImportError as error:
        print("qiskit-qasm3-import is required for downstream conversion validation", file=sys.stderr)
        print(error, file=sys.stderr)
        return 2

    try:
        circuit = parse(fixture.read_text(encoding="utf-8"))
    except Exception as error:  # The downstream importer exposes implementation-specific errors.
        print(f"Qiskit OpenQASM 3 importer rejected {fixture}: {error}", file=sys.stderr)
        return 1

    expected_operations = ["h", "cx", "h", "cx", "rx", "ry", "rz", "p", "measure"]
    actual_operations = [instruction.operation.name for instruction in circuit.data]
    if circuit.num_qubits != 6 or circuit.num_clbits != 6 or actual_operations != expected_operations:
        print(
            f"unexpected imported circuit: qubits={circuit.num_qubits}, clbits={circuit.num_clbits}, operations={actual_operations}",
            file=sys.stderr,
        )
        return 1

    print(f"Qiskit OpenQASM 3 importer converted {fixture} into the expected {circuit.num_qubits}-qubit circuit")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
