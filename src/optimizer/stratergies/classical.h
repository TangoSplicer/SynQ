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
// File: optimizer/strategies/classical.h
#pragma once

#include <map>
#include <string>
#include <functional>

namespace synq {
namespace classical {

using ParamMap = std::map<std::string, double>;
using Objective = std::function<double(const ParamMap&)>;

// Standard optimizers
ParamMap gridSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap randomSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap nelderMeadSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap evolutionSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap bracketedSearch(const Objective& f, const ParamMap& init, int max_iters);

// Unique optimizer
ParamMap echoWaveSearch(const Objective& f, const ParamMap& init, int max_iters);

} // namespace classical
} // namespace synq