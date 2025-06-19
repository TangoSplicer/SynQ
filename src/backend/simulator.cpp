// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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