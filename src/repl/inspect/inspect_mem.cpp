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
// synq/src/repl/inspect/inspect_mem.cpp

#include <iostream>
#include <map>
#include <string>

void inspect_memory() {
    std::map<std::string, std::string> classical = {
        {"x", "42"}, {"flag", "true"}
    };
    std::map<std::string, std::string> quantum = {
        {"q0", "|+>"}, {"q1", "|1>"}
    };
    std::map<std::string, std::string> plugin = {
        {"visual_mode", "enabled"}, {"theme", "dark"}
    };

    std::cout << "🔍 Inspecting Memory State\n";

    std::cout << "\n📦 Classical Memory:\n";
    for (const auto& [k, v] : classical) std::cout << "  " << k << " = " << v << "\n";

    std::cout << "\n⚛️ Quantum Memory:\n";
    for (const auto& [k, v] : quantum) std::cout << "  " << k << " = " << v << "\n";

    std::cout << "\n🔌 Plugin Memory:\n";
    for (const auto& [k, v] : plugin) std::cout << "  " << k << " = " << v << "\n";
}