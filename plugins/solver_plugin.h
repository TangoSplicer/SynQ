// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/solver_plugin.h
#pragma once

#include "plugin.h"
#include <string>
#include <map>

namespace synq {

// Interface for SAT/CVX/AI solvers
class SolverPlugin : public Plugin {
public:
    // Input parameters, returns result as string (solution or reason)
    virtual std::string solve(const std::map<std::string, double>& inputs) = 0;
};

} // namespace synq