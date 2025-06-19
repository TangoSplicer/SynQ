// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/quantum/qaoa.cpp

#include "qaoa.h"
#include "../classical.h"
#include "../../optimizer.h"
#include <iostream>

namespace synq {
namespace quantum {

ParamMap runQAOA(
    const CostFunction& cost,
    const ParamMap& initial,
    int steps,
    const std::string& strategy,
    int max_iters
) {
    std::cout << "[QAOA] Optimizing QAOA parameters (γ, β)...\n";

    Optimizer::ObjectiveFunction obj = [=](const ParamMap& p) {
        return cost(p); // externally defined cost function (e.g., expectation)
    };

    synq::Optimizer opt;
    ParamMap best = opt.optimize(obj, initial, strategy, max_iters);

    std::cout << "[QAOA] Optimized γ=" << best.at("gamma")
              << ", β=" << best.at("beta") << "\n";
    return best;
}

} // namespace quantum
} // namespace synq