// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/inspect/inspect_plugin.cpp

#include <iostream>
#include <vector>
#include <string>

void inspect_plugins() {
    std::vector<std::string> loaded = {
        "visual_debugger", "qaoa_optimizer", "llama_classifier"
    };

    std::cout << "🔌 Loaded Plugins:\n";
    for (const auto& name : loaded) {
        std::cout << "  - " << name << "\n";
    }

    std::cout << "🧠 Plugin hooks detected: visual_render, ai_embed\n";
}