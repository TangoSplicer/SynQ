// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "quantum_prompt.h"
#include <iostream>

namespace synq {

void QuantumPrompt::renderCircuit(const std::string& qcode) {
    std::cout << "[QuantumPrompt] Simulated quantum circuit:\n";
    std::cout << "  Q0 ──H──●──X── " << qcode << "\n";
    std::cout << "         │\n";
    std::cout << "  Q1 ────■────── \n";
}

void QuantumPrompt::visualizeState(const std::string& stateVec) {
    std::cout << "[QuantumPrompt] State Vector Snapshot: " << stateVec << "\n";
}

}