// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "error_dive.h"
#include "debug_trace_tree.h"
#include <iostream>
#include <chrono>

namespace synq {

void ErrorDive::dive(const std::string& errorMsg) {
    using namespace std::chrono;
    auto now = system_clock::to_time_t(system_clock::now());

    std::cout << "\n[ErrorDive] Runtime Exception Captured\n";
    std::cout << "  > Message: " << errorMsg << "\n";
    std::cout << "  > Timestamp: " << std::ctime(&now);
    std::cout << "\n[ErrorDive] Executed Call Trace:\n";
    DebugTraceTree::printTrace();

    std::cout << "[ErrorDive] Suggestion: Run `showAll()` to inspect state.\n";
}

}