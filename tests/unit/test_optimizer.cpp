// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: tests/unit/test_optimizer.cpp

#include "../../src/runtime/runtime.h"
#include "../../optimizer/optimizer.h"
#include "../../optimizer/strategies/classical.h"
#include <iostream>
#include <cmath>

using namespace synq;
using namespace classical;

double testFunction(const ParamMap& params) {
    double x = params.at("x");
    return (x - 1.0) * (x - 1.0); // min at x = 1
}

int main() {
    std::cout << "[TEST] Classical Optimizer EchoWave\n";

    ParamMap start = { {"x", 3.0} };
    auto result = echoWaveSearch(testFunction, start, 25);

    std::cout << "Result x = " << result["x"] << "\n";

    if (std::abs(result["x"] - 1.0) < 0.2) {
        std::cout << "[PASS] EchoWave converged to minimum.\n";
        return 0;
    } else {
        std::cerr << "[FAIL] EchoWave did not converge properly.\n";
        return 1;
    }
}