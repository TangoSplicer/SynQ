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
#include "eval_context.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> EvalContext::variables;

void EvalContext::define(const std::string& var, const std::string& val) {
    variables[var] = val;
}

std::string EvalContext::resolve(const std::string& var) {
    return variables.count(var) ? variables[var] : "[undefined]";
}

void EvalContext::dump() {
    std::cout << "[EvalContext Dump]\n";
    for (const auto& [k, v] : variables) {
        std::cout << "  " << k << " = " << v << "\n";
    }
}

void EvalContext::clear() {
    variables.clear();
}

   std::map<std::string, std::string> EvalContext::getVariables() {
       return variables;
   }

}