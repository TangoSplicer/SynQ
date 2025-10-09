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
#include "errorwhisperer.h"
#include <iostream>

namespace synq {

std::vector<ErrorWhisperer::ErrorEntry> ErrorWhisperer::history;

void ErrorWhisperer::recordError(const std::string& message, const std::string& file, int line) {
    history.push_back({message, file, line});
}

std::string ErrorWhisperer::explainLastError() {
    if (history.empty()) return "No errors recorded.";
    const auto& err = history.back();
    return "Error: " + err.message + " at " + err.file + ":" + std::to_string(err.line);
}

void ErrorWhisperer::suggestFixes() {
    if (history.empty()) return;
    std::cout << "[ErrorWhisperer Suggestions]\n";
    std::cout << "  - Check for missing semicolons or type mismatches.\n";
    std::cout << "  - Use SynTrace or PulseWatch to trace runtime state.\n";
    std::cout << "  - Run symbolic analyzer for type or logic conflict resolution.\n";
}

void ErrorWhisperer::clear() {
    history.clear();
}

}