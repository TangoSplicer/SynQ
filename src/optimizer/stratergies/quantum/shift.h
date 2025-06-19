// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/quantum/shift.h
#pragma once

#include <map>
#include <string>
#include <functional>

namespace synq {
namespace quantum {

using ParamMap = std::map<std::string, double>;
using ObservableFunction = std::function<double(const ParamMap&)>;

// Estimate gradients for parameters using the parameter-shift rule
ParamMap parameterShiftGradient(
    const ObservableFunction& observable,
    const ParamMap& params,
    double shift = 0.1
);

} // namespace quantum
} // namespace synq