// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/runtime/coverage.h
#pragma once

#include <map>
#include <string>
#include <fstream>

namespace synq {

class CoverageTracker {
private:
    std::map<std::string, int> line_hits;
    std::map<std::string, int> function_calls;

public:
    void markLine(const std::string& label);
    void markFunction(const std::string& func);

    void writeReport(const std::string& filename = "coverage/coverage_report.txt") const;
    void clear();
};

extern CoverageTracker coverage;

} // namespace synq