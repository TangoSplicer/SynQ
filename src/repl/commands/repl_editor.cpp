// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_editor.cpp

#include <iostream>
#include <string>

extern std::vector<std::string> trace_log;

void edit_last_command() {
    if (trace_log.empty()) {
        std::cout << "⚠️ Nothing to edit.\n";
        return;
    }

    std::string last = trace_log.back();
    std::string modified;

    std::cout << "📝 Editing last line:\n" << last << "\n";
    std::cout << "New> ";
    std::getline(std::cin, modified);

    trace_log.pop_back();
    trace_log.push_back(modified);

    std::cout << "✔️ Updated.\n";
}