// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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