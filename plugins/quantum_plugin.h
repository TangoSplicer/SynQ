// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/quantum_plugin.h
#pragma once

#include "plugin.h"
#include <vector>
#include <string>

namespace synq {

// Interface for pluggable quantum simulators or QPU backends
class QuantumSimulatorPlugin : public Plugin {
public:
    virtual void allocate(const std::string& name) = 0;
    virtual void applyGate(const std::string& gate, const std::vector<std::string>& targets) = 0;
    virtual std::vector<int> measureAll() = 0;
    virtual std::string getState() const = 0;
};

} // namespace synq