// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_config.cpp

#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> repl_config = {
    {"ai_model", "gpt-4"},
    {"theme", "dark"},
    {"autosave", "on"}
};

void show_config() {
    std::cout << "⚙️ Current REPL Configuration:\n";
    for (const auto& [key, val] : repl_config)
        std::cout << "  " << key << ": " << val << "\n";
}