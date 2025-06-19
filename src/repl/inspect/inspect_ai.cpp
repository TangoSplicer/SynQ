// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/inspect/inspect_ai.cpp

#include <iostream>
#include <vector>

void inspect_ai_usage() {
    std::vector<std::string> prompts = {
        "Explain entanglement",
        "Generate circuit for teleportation",
        "What is Grover's algorithm?"
    };

    std::cout << "🤖 AI Prompt History:\n";
    for (const auto& p : prompts) {
        std::cout << "  📝 " << p << "\n";
    }

    std::cout << "📊 Total tokens used: 391\n";
    std::cout << "🧠 Last model used: GPT-4 (OpenAI)\n";
}