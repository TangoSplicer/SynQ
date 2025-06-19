// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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