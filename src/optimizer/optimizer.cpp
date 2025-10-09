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
// File: optimizer/optimizer.cpp

#include "optimizer.h"
#include "stratergies/classical.h"
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