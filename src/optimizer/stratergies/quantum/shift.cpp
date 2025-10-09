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