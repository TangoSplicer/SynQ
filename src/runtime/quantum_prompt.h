// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_QUANTUM_PROMPT_H
#define SYNQ_QUANTUM_PROMPT_H

#include <string>

namespace synq {

class QuantumPrompt {
public:
    static void renderCircuit(const std::string& qcode);
    static void visualizeState(const std::string& stateVec);
};

}

#endif // SYNQ_QUANTUM_PROMPT_H