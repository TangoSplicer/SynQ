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
// synq/src/cli/synq_skillmap.cpp

#include <iostream>
#include <map>

void generate_skillmap() {
    std::map<std::string, int> usage = {
        {"Qubit", 18}, {"QAOA", 5}, {"AI", 3}, {"Plugin", 8}
    };

    std::cout << "🧠 Skill Heatmap:\n";
    for (const auto& [k, v] : usage) {
        std::cout << k << ": ";
        for (int i = 0; i < v; ++i) std::cout << "█";
        std::cout << " (" << v << ")\n";
    }

    std::cout << "💡 Try using underused features to improve coverage.\n";
}

int main() {
    generate_skillmap();
    return 0;
}