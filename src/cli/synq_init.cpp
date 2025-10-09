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
// synq/src/cli/synq_init.cpp

#include <iostream>
#include <filesystem>
#include <fstream>

void write_file(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (file) {
        file << content;
        file.close();
    }
}

int main() {
    std::cout << "Would you like to install developer aliases + shell completions? [Y/n]: ";
    std::string ans;
    std::getline(std::cin, ans);
    if (ans.empty() || ans == "Y" || ans == "y") {
        system("bash synq/scripts/install_tools.sh");
        std::cout << "🔁 Developer tools installed.\n";
    }
    
    std::string base = "my_synq_project";
    std::filesystem::create_directories(base + "/src");
    std::filesystem::create_directories(base + "/tests");
    std::filesystem::create_directories(base + "/docs");

    write_file(base + "/src/main.synq", "// Main SynQ source file\n");
    write_file(base + "/README.md", "# My SynQ Project\n");
    write_file(base + "/tests/test_main.synq", "// Unit tests go here\n");

    std::cout << "📁 Initialized new SynQ project in 'my_synq_project/'\n";
    return 0;
}