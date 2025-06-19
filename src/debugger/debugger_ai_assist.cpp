// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debugger_ai_assist.h"
#include <iostream>

namespace synq {

void DebuggerAIAssist::explain(const std::string& errorMsg) {
    std::cout << "\n[AI Assist] Analyzing error: \"" << errorMsg << "\"\n";

    // Simulated "friendly" explanation based on common error keywords
    if (errorMsg.find("undefined") != std::string::npos) {
        std::cout << "[AI Assist] Looks like you're referencing a variable that hasn't been declared.\n";
    } else if (errorMsg.find("type") != std::string::npos) {
        std::cout << "[AI Assist] This might be a type mismatch. Did you try to add a string to an integer?\n";
    } else if (errorMsg.find("division") != std::string::npos) {
        std::cout << "[AI Assist] Division by zero! Double-check your denominator.\n";
    } else {
        std::cout << "[AI Assist] Hmm… not sure exactly, but check for syntax or logic issues near the error.\n";
    }

    std::cout << "[AI Assist] Want more help? Try `explain_verbose(error)` in REPL.\n";
}

}