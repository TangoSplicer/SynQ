// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/visualizer_plugin.h
#pragma once

#include "plugin.h"
#include <string>

namespace synq {

// Interface for plugins that visualize circuit states or flows
class VisualizerPlugin : public Plugin {
public:
    virtual void visualizeCircuit(const std::string& serialized_circuit) = 0;
    virtual void showMeasurement(const std::vector<int>& result) = 0;
};

} // namespace synq