// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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