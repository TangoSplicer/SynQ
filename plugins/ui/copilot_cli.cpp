// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "../../compiler/repl.h"
#include "../copilot_agent.h"
#include <iostream>

using namespace synq;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./copilot_cli <mode> <plugin> [payload_json]\n";
        return 1;
    }

    std::string mode = argv[1];
    std::string plugin = argv[2];
    std::string payload_raw = (argc > 3) ? argv[3] : "{}";

    REPL repl;
    CopilotAgent agent(repl);
    agent.setMode(mode);

    nlohmann::json payload;
    try {
        payload = nlohmann::json::parse(payload_raw);
    } catch (...) {
        std::cerr << "[Error] Invalid JSON payload.\n";
        return 1;
    }

    std::string output = agent.handleUserIntent(plugin, payload);
    std::cout << output << std::endl;
    return 0;
}