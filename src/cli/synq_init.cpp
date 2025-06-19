// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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