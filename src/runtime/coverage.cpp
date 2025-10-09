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
// File: src/runtime/coverage.cpp

#include "coverage.h"
#include <iostream>
#include <filesystem>

namespace synq {

CoverageTracker coverage;

void CoverageTracker::markLine(const std::string& label) {
    line_hits[label]++;
}

void CoverageTracker::markFunction(const std::string& func) {
    function_calls[func]++;
}

void CoverageTracker::writeReport(const std::string& filename) const {
    std::filesystem::create_directories("coverage");
    std::ofstream out(filename);

    out << "==== SynQ Coverage Report ====\n\n";

    out << "[Line Coverage]\n";
    for (const auto& [label, count] : line_hits) {
        out << "  " << label << ": " << count << " hit(s)\n";
    }

    out << "\n[Function Calls]\n";
    for (const auto& [func, count] : function_calls) {
        out << "  " << func << "(): " << count << " call(s)\n";
    }

    out.close();
}

void CoverageTracker::clear() {
    line_hits.clear();
    function_calls.clear();
}

} // namespace synq