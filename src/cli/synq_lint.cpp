// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_lint.cpp

#include <iostream>
#include <fstream>
#include <regex>

void lint_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "❌ Cannot open file: " << path << "\n";
        return;
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