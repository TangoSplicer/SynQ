// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/core/repl_trace.cpp

#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> trace_log;

void log_trace_step(const std::string& step) {
    trace_log.push_back(step);
}

void show_trace() {
    std::cout << "🧭 Execution Trace Log:\n";
    for (const auto& step : trace_log) {
        std::cout << " - " << step << "\n";
    }
}