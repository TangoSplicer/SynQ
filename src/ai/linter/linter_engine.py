# MIT License
#
# Copyright (c) 2025 SynQ Contributors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# © 2025 SynQ Contributors. All rights reserved.
# File: Quantum Circuit Linter Engine

import json
from typing import Dict, List

class LintWarning:
    def __init__(self, code: str, message: str, severity: str = "warning"):
        self.code = code
        self.message = message
        self.severity = severity

    def to_dict(self):
        return {
            "code": self.code,
            "message": self.message,
            "severity": self.severity
        }

def lint_ir(ir: Dict) -> List[LintWarning]:
    """
    Given QIR (Quantum Intermediate Representation), return list of warnings.
    """
    warnings = []

    qubit_use = {}
    depth_map = {}
    last_gate_by_qubit = {}

    for gate in ir.get("gates", []):
        gate_name = gate["name"]
        targets = gate.get("targets", [])
        controls = gate.get("controls", [])
        time = gate.get("timestep", 0)

        # ⚠️ Check for deprecated or discouraged gates
        if gate_name in ["swap", "iswap", "cswap"]:
            warnings.append(LintWarning("LINT_DEPRECATED_SWAP",
                                        f"Use of {gate_name} is discouraged for topology compatibility.",
                                        "info"))

        # ⚠️ Check for classical-conditioned quantum logic
        if gate.get("condition") is not None:
            warnings.append(LintWarning("LINT_CLASSICAL_COND",
                                        f"{gate_name} is conditionally applied based on classical register.",
                                        "warning"))

        # 🌀 Check for overlapping gates on same qubit
        for q in targets + controls:
            if q in last_gate_by_qubit and abs(time - last_gate_by_qubit[q]) < 2:
                warnings.append(LintWarning("LINT_QUBIT_COLLISION",
                                            f"Qubit {q} used in overlapping gates at t={time}",
                                            "error"))
            last_gate_by_qubit[q] = time

        # 📊 Count qubit frequency
        for q in targets:
            qubit_use[q] = qubit_use.get(q, 0) + 1

        # ⏱ Depth tracking
        for q in targets + controls:
            depth_map[q] = max(depth_map.get(q, 0), time)

    # 🚨 Excessive qubit reuse
    for q, count in qubit_use.items():
        if count > 12:
            warnings.append(LintWarning("LINT_QUBIT_OVERUSE",
                                        f"Qubit {q} used {count} times – consider reallocation.",
                                        "info"))

    # ⏳ Depth bottlenecks
    high_depth = [q for q, d in depth_map.items() if d > 50]
    if len(high_depth) > 0:
        warnings.append(LintWarning("LINT_CIRCUIT_DEPTH",
                                    f"High logical depth on {len(high_depth)} qubits (t > 50)",
                                    "warning"))

    return warnings

def print_lint_report(ir: Dict):
    print("🔍 Linting quantum circuit...\n")
    warnings = lint_ir(ir)
    for w in warnings:
        icon = "❌" if w.severity == "error" else ("⚠️" if w.severity == "warning" else "ℹ️")
        print(f"{icon} [{w.code}] {w.message}")
    if not warnings:
        print("✅ No linting issues found.")