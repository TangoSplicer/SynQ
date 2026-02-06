// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// File: src/compiler/transpiler.cpp

#include "transpiler.h"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace synq {

Transpiler::Transpiler() {}

void Transpiler::load_ir(const std::string& ir_code) {
    ir = ir_code;
    tokenize();
}

void Transpiler::tokenize() {
    std::istringstream stream(ir);
    std::string token;
    tokens.clear();
    while (stream >> token) {
        tokens.push_back(token);
    }
}

std::string Transpiler::export_to(TargetFormat format) {
    switch (format) {
        case TargetFormat::QASM: return to_qasm();
        case TargetFormat::CIRQ: return to_cirq();
        case TargetFormat::BRAKET: return to_braket();
        default: return "// Unknown format";
    }
}

std::string Transpiler::to_qasm() {
    std::ostringstream out;
    out << "OPENQASM 2.0;\ninclude \"qelib1.inc\";\n";
    out << "qreg q[10];\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "H") out << "h q[" << tokens[++i] << "];\n";
        else if (tokens[i] == "CNOT") {
            int ctrl = std::stoi(tokens[++i]);
            int tgt = std::stoi(tokens[++i]);
            out << "cx q[" << ctrl << "],q[" << tgt << "];\n";
        }
    }

    return out.str();
}

std::string Transpiler::to_cirq() {
    std::ostringstream out;
    out << "import cirq\n\n";
    out << "qubits = [cirq.LineQubit(i) for i in range(10)]\n";
    out << "circuit = cirq.Circuit()\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "H") out << "circuit.append(cirq.H(qubits[" << tokens[++i] << "]))\n";
        else if (tokens[i] == "CNOT") {
            int ctrl = std::stoi(tokens[++i]);
            int tgt = std::stoi(tokens[++i]);
            out << "circuit.append(cirq.CNOT(qubits[" << ctrl << "], qubits[" << tgt << "]))\n";
        }
    }

    out << "\nprint(circuit)";
    return out.str();
}

std::string Transpiler::to_braket() {
    std::ostringstream out;
    out << "from braket.circuits import Circuit\n";
    out << "circuit = Circuit()\n";

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "H") out << "circuit.h(" << tokens[++i] << ")\n";
        else if (tokens[i] == "CNOT") {
            int ctrl = std::stoi(tokens[++i]);
            int tgt = std::stoi(tokens[++i]);
            out << "circuit.cnot(" << ctrl << ", " << tgt << ")\n";
        }
    }

    out << "\nprint(circuit)";
    return out.str();
}

} // namespace synq