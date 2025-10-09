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
// File: backend/simulator.cpp

#include "simulator.h"
#include <iostream>
#include <sstream>

namespace synq {

QuantumSimulator::QuantumSimulator() {
    // Initialize with default state
    state = {{1.0, 0.0}}; // |0⟩
}

void QuantumSimulator::allocateQubit(const std::string& name) {
    if (qubit_map.count(name) == 0) {
        qubit_map[name] = qubit_order.size();
        qubit_order.push_back(name);
        std::cout << "[SIM] Allocated qubit: " << name << "\n";
    }
}

void QuantumSimulator::applyGate(const std::string& gate, const std::vector<std::string>& targets) {
    std::cout << "[SIM] Applied " << gate << " on ";
    for (auto& t : targets) std::cout << t << " ";
    std::cout << "\n";
    // (placeholder logic)
}

std::vector<int> QuantumSimulator::measureAll() {
    std::cout << "[SIM] Measurement triggered\n";
    return {0, 1}; // mock binary result
}

std::string QuantumSimulator::serialize() const {
    std::ostringstream oss;
    oss << "Circuit with " << qubit_order.size() << " qubits:\n";
    for (auto& q : qubit_order) {
        oss << "  • " << q << "\n";
    }
    return oss.str();
}

std::vector<double> QuantumSimulator::getStatevector() const {
    return {1.0, 0.0}; // dummy placeholder
}

} // namespace synq