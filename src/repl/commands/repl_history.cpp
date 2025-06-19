// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_history.cpp

#include <iostream>
#include <vector>

extern std::vector<std::string> trace_log;
extern std::vector<std::string> ai_history;

void show_history() {
    std::cout << "🧾 Command History:\n";
    for (const auto& line : trace_log)
        std::cout << " >> " << line << "\n";

    std::cout << "\n🤖 AI Prompts:\n";
    for (const auto& prompt : ai_history)
        std::cout << " 🧠 " << prompt << "\n";
}