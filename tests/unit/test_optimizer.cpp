// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// File: tests/unit/test_optimizer.cpp

#include "../../src/runtime/runtime.h"
#include "optimizer/optimizer.h"
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