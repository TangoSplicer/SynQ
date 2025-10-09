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