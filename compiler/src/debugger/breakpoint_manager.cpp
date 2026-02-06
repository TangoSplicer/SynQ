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
#include "breakpoint_manager.h"
#include <iostream>

namespace synq {

std::map<std::string, std::map<int, bool>> BreakpointManager::breakpoints;

void BreakpointManager::add(const std::string& file, int line) {
    breakpoints[file][line] = true;
}

void BreakpointManager::remove(const std::string& file, int line) {
    breakpoints[file].erase(line);
}

bool BreakpointManager::isActive(const std::string& file, int line) {
    return breakpoints[file].count(line) > 0 && breakpoints[file][line];
}

void BreakpointManager::toggle(const std::string& file, int line) {
    breakpoints[file][line] = !breakpoints[file][line];
}

void BreakpointManager::list() {
    std::cout << "[Breakpoints]\n";
    for (const auto& [file, lines] : breakpoints) {
        for (const auto& [line, active] : lines) {
            std::cout << "  " << file << ":" << line << (active ? " [ON]" : " [OFF]") << "\n";
        }
    }
}

}