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
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> readLines(const std::string& path) {
    std::ifstream file(path);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) lines.push_back(line);
    return lines;
}

void diffAudit(const std::vector<std::string>& oldCode, const std::vector<std::string>& newCode) {
    size_t max = std::max(oldCode.size(), newCode.size());
    for (size_t i = 0; i < max; ++i) {
        std::string oldLine = i < oldCode.size() ? oldCode[i] : "";
        std::string newLine = i < newCode.size() ? newCode[i] : "";
        if (oldLine != newLine) {
            std::cout << "- " << oldLine << "\n+ " << newLine << "\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./copilot_diff_audit <old_file> <new_file>\n";
        return 1;
    }

    auto oldCode = readLines(argv[1]);
    auto newCode = readLines(argv[2]);
    std::cout << "🔍 Diff between " << argv[1] << " and " << argv[2] << ":\n";
    diffAudit(oldCode, newCode);

    return 0;
}