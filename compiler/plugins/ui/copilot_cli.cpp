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