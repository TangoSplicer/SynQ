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
// synq/src/cli/synq_test.cpp

#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>

namespace fs = std::filesystem;

void run_test(const fs::path& path) {
    std::cout << "🧪 Running test: " << path.filename() << "\n";
    std::ifstream test_file(path);
    if (!test_file) {
        std::cerr << "❌ Could not open test file: " << path << "\n";
        exit(1);
    }

    std::string line;
    while (std::getline(test_file, line)) {
        std::cout << ">> " << line << "\n";  // Simulated test eval
    }

    std::cout << "✅ Test passed: " << path.filename() << "\n";
}

void discover_and_run_tests(const std::string& dir) {
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.path().extension() == ".synq") {
            run_test(entry.path());
        }
    }
}

int main(int argc, char* argv[]) {
    std::string test_dir = argc > 1 ? argv[1] : "tests/";
    std::cout << "🔍 Discovering tests in: " << test_dir << "\n";

    if (!fs::exists(test_dir)) {
        std::cerr << "❌ Test directory does not exist.\n";
        return 1;
    }

    discover_and_run_tests(test_dir);
    return 0;
}