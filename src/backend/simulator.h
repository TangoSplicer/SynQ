// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: backend/simulator.h
#pragma once

#include <vector>
#include <string>
#include <map>

namespace synq {

class QuantumSimulator {
public:
    QuantumSimulator();

    void allocateQubit(const std::string& name);
    void applyGate(const std::string& gate, const std::vector<std::string>& targets);
    std::vector<int> measureAll();
    std::string serialize() const;
    std::vector<double> getStatevector() const;

private:
    std::vector<std::string> qubit_order;
    std::map<std::string, int> qubit_map;
    std::vector<std::vector<double>> state; // placeholder matrix
};

} // namespace synq