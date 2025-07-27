// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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