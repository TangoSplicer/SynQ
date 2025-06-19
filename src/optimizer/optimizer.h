// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/optimizer.h
#pragma once

#include <functional>
#include <vector>
#include <string>
#include <map>

namespace synq {

class Optimizer {
public:
    using ObjectiveFunction = std::function<double(const std::map<std::string, double>&)>;

    // Run optimization with initial parameters and selected strategy
    std::map<std::string, double> optimize(
        const ObjectiveFunction& f,
        const std::map<std::string, double>& init,
        const std::string& strategy = "grid",
        int max_iters = 100
    );
};

} // namespace synq