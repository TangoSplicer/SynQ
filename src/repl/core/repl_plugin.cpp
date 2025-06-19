// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/core/repl_plugin.cpp

#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> plugin_context;

void handle_plugin_command(const std::string& cmd) {
    std::cout << "🔌 Plugin control: " << cmd << "\n";
    plugin_context["last"] = cmd;

    if (cmd == "load visual_debugger") {
        std::cout << "✔️ Plugin `visual_debugger` loaded.\n";
    } else if (cmd == "mutate current") {
        std::cout << "🧬 Mutating active plugin...\n";
    }
}