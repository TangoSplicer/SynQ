// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_plot.cpp

#include <iostream>
#include <vector>
#include <string>

extern std::vector<std::pair<std::string, double>> amplitudes;

void plot_entanglement() {
    std::cout << "📈 Entanglement Amplitude Plot:\n";
    for (const auto& [state, amp] : amplitudes) {
        std::cout << state << ": ";
        int bars = static_cast<int>(amp * 20);
        for (int i = 0; i < bars; ++i) std::cout << "█";
        std::cout << " (" << amp << ")\n";
    }
}