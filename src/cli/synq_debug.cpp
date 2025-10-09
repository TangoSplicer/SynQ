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
// synq/src/cli/synq_debug.cpp

#include <iostream>
#include <fstream>

void debug_script(const std::string& file) {
    std::cout << "🐞 Debugging file: " << file << "\n";

    std::ifstream src(file);
    if (!src) {
        std::cerr << "❌ File not found: " << file << "\n";
        exit(1);
    }

    std::string line;
    int lineno = 1;
    while (std::getline(src, line)) {
        std::cout << "[LINE " << lineno++ << "] >> " << line << "\n";
    }

    std::cout << "🔍 Use :break, :step, :mem in REPL for detailed inspection.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq debug <file.synq>\n";
        return 1;
    }

    debug_script(argv[1]);
    return 0;
}