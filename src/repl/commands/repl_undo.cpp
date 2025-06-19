// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_undo.cpp

#include <iostream>
#include <vector>
#include <string>

extern std::vector<std::string> trace_log;

void undo_last_action() {
    if (!trace_log.empty()) {
        std::cout << "↩️ Undo: " << trace_log.back() << "\n";
        trace_log.pop_back();
    } else {
        std::cout << "🛑 Nothing to undo.\n";
    }
}