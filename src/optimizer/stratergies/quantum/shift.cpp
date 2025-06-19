// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/quantum/shift.cpp

#include "shift.h"
#include <iostream>

namespace synq {
namespace quantum {

ParamMap parameterShiftGradient(
    const ObservableFunction& observable,
    const ParamMap& params,
    double shift
) {
    ParamMap gradient;

    for (const auto& [param, value] : params) {
        ParamMap forward = params;
        ParamMap backward = params;

        forward[param] = value + shift;
        backward[param] = value - shift;

        double f_plus = observable(forward);
        double f_minus = observable(backward);

        double grad = (f_plus - f_minus) / (2.0 * shift);
        gradient[param] = grad;

        std::cout << "[SHIFT] ∂/∂" << param << " = " << grad << "\n";
    }

    return gradient;
}

} // namespace quantum
} // namespace synq