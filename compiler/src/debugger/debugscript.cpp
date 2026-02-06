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
#include "debugscript.h"
#include <iostream>
#include <sstream>

namespace synq {

std::vector<std::string> DebugScript::scriptLines;

void DebugScript::load(const std::string& script) {
    std::istringstream ss(script);
    std::string line;
    while (std::getline(ss, line)) {
        scriptLines.push_back(line);
    }
}

void DebugScript::run() {
    std::cout << "[DebugScript Executing...]\n";
    for (const auto& line : scriptLines) {
        std::cout << "-> " << line << "\n";
        // Hook for evaluating symbolic conditions or triggers
    }
}

void DebugScript::clear() {
    scriptLines.clear();
}

}