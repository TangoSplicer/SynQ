// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/core/repl_shell.cpp

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

extern void handle_ai_prompt(const std::string&);
extern void handle_plugin_command(const std::string&);
extern void log_trace_step(const std::string&);
extern void snapshot_state();

void execute_line(const std::string& line) {
    if (line.starts_with(":@")) handle_ai_prompt(line.substr(2));
    else if (line.starts_with(":=")) handle_plugin_command(line.substr(2));
    else if (line == ":snapshot") snapshot_state();
    else {
        std::cout << "🧪 Evaluating: " << line << "\n";
        log_trace_step(line);
    }
}

void start_shell_loop() {
    std::string input;
    while (true) {
        std::cout << "synq>> ";
        if (!std::getline(std::cin, input)) break;
        if (input == "exit" || input == "quit") break;
        execute_line(input);
    }
}