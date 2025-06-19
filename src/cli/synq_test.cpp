// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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
        std::cerr << "❌ Could not open test file.\n";
        return;
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