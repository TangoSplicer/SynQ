// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debug_watch.h"
#include <iostream>
#include <set>

namespace synq {

static std::set<std::string> watchedVars;

void DebugWatch::watch(const std::string& variable) {
    watchedVars.insert(variable);
    std::cout << "[DebugWatch] Watching: " << variable << "\n";
}

void DebugWatch::unwatch(const std::string& variable) {
    watchedVars.erase(variable);
    std::cout << "[DebugWatch] Unwatched: " << variable << "\n";
}

void DebugWatch::list() {
    std::cout << "[DebugWatch] Current watches:\n";
    for (const auto& var : watchedVars) {
        std::cout << " - " << var << "\n";
    }
}

}