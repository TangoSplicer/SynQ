// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "symbol_visualizer.h"
#include "eval_context.h"

#include <iostream>
#include <iomanip>

namespace synq {

void SymbolVisualizer::showAll() {
    std::cout << "[Visualizer] Current Variable Context:\n";
    std::cout << std::setw(16) << "Name" << std::setw(20) << "Value" << "\n";
    std::cout << std::string(36, '-') << "\n";

    for (const auto& [k, v] : EvalContext::variables) {
        std::cout << std::setw(16) << k << std::setw(20) << v << "\n";
    }
}

void SymbolVisualizer::show(const std::string& name) {
    auto it = EvalContext::variables.find(name);
    if (it != EvalContext::variables.end()) {
        std::cout << "[Visualizer] " << name << " = " << it->second << "\n";
    } else {
        std::cout << "[Visualizer] No such symbol: " << name << "\n";
    }
}

}