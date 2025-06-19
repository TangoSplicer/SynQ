// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/optimizer.cpp

#include "optimizer.h"
#include "strategies/classical.h"
#include <iostream>

namespace synq {

std::map<std::string, double> Optimizer::optimize(
    const ObjectiveFunction& f,
    const std::map<std::string, double>& init,
    const std::string& strategy,
    int max_iters
) {
    std::cout << "[OPT] Strategy: " << strategy << ", Max Iters: " << max_iters << "\n";

    if (strategy == "grid") {
        return classical::gridSearch(f, init, max_iters);
    } else if (strategy == "random") {
        return classical::randomSearch(f, init, max_iters);
    } else {
        std::cerr << "[OPT] Unknown strategy: " << strategy << "\n";
        return init;
    }
}

} // namespace synq