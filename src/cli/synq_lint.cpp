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
// synq/src/cli/synq_lint.cpp

#include <iostream>
#include <fstream>
#include <regex>

void lint_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "❌ Cannot open file: " << path << "\n";
        exit(1);
    }

    std::string line;
    int line_number = 0;
    std::cout << "🧹 Linting " << path << "...\n";

    while (std::getline(file, line)) {
        ++line_number;
        if (std::regex_search(line, std::regex("\\bvar\\b"))) {
            std::cout << "⚠️  Line " << line_number << ": Avoid using 'var'; use explicit type.\n";
        }
        if (line.find("== true") != std::string::npos) {
            std::cout << "⚠️  Line " << line_number << ": Simplify '== true' to conditional.\n";
        }
    }

    std::cout << "✅ Linting complete.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq lint <file.synq>\n";
        return 1;
    }

    lint_file(argv[1]);
    return 0;
}