// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/inspect/inspect_heat.cpp

#include <iostream>
#include <map>

void show_heatmap() {
    std::map<std::string, int> usage = {
        {"Quantum Gates", 15},
        {"AI Prompts", 7},
        {"Memory Calls", 5},
        {"Plugin Loads", 9}
    };

    std::cout << "🔥 Feature Heatmap:\n";
    for (const auto& [feature, count] : usage) {
        std::cout << feature << ": ";
        for (int i = 0; i < count; ++i) std::cout << "█";
        std::cout << " (" << count << ")\n";
    }
}