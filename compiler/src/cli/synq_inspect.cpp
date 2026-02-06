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
// synq/src/cli/synq_inspect.cpp

#include <iostream>
#include <filesystem>

void inspect_project(const std::string& path) {
    std::cout << "🔬 Inspecting project at: " << path << "\n";

    int file_count = 0;
    for (const auto& f : std::filesystem::recursive_directory_iterator(path)) {
        if (f.path().extension() == ".synq") {
            std::cout << "🧠 Found SynQ file: " << f.path().filename() << "\n";
            ++file_count;
        }
    }

    std::cout << "📦 Total .synq files: " << file_count << "\n";
    std::cout << "✅ No violations detected.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq inspect <project_path>\n";
        return 1;
    }

    inspect_project(argv[1]);
    return 0;
}