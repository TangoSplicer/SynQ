// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "shadow_runtime.h"
#include <iostream>

namespace synq {

std::vector<std::string> ShadowRuntime::failedPaths;

void ShadowRuntime::recordFailedPath(const std::string& code) {
    failedPaths.push_back(code);
}

std::string ShadowRuntime::suggestRecovery() {
    if (failedPaths.empty()) return "[No failed paths]";
    return "Retry with modified parameters or reduced complexity.";
}

void ShadowRuntime::printFailedPaths() {
    std::cout << "[ShadowRuntime] Failed paths:\n";
    for (const auto& code : failedPaths) {
        std::cout << "  >> " << code << "\n";
    }
}

}