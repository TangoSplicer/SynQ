// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/quantum/variational.cpp

#include "variational.h"
#include "../classical.h"
#include "../../optimizer.h"
#include <iostream>

namespace synq {
namespace quantum {

ParamMap runVariational(
    const EnergyFunction& cost,
    const ParamMap& initial,
    const std::string& strategy,
    int max_iters
) {
    std::cout << "[VQE] Running variational energy minimization...\n";

    // Wrap the energy function in a classical ObjectiveFunction
    Optimizer::ObjectiveFunction obj = [=](const ParamMap& p) {
        return cost(p); // call expectation evaluator
    };

    synq::Optimizer opt;
    ParamMap result = opt.optimize(obj, initial, strategy, max_iters);

    std::cout << "[VQE] Final energy: " << cost(result) << "\n";
    return result;
}

} // namespace quantum
} // namespace synq