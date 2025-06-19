// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/quantum/qaoa.h
#pragma once

#include <map>
#include <string>
#include <functional>

namespace synq {
namespace quantum {

using ParamMap = std::map<std::string, double>;
using CostFunction = std::function<double(const ParamMap&)>;

// Runs a simple QAOA loop over γ, β parameters
ParamMap runQAOA(
    const CostFunction& cost,
    const ParamMap& initial,
    int steps = 1,
    const std::string& strategy = "bracketed",
    int max_iters = 40
);

} // namespace quantum
} // namespace synq