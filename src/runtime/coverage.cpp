// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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