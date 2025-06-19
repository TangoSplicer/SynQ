// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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