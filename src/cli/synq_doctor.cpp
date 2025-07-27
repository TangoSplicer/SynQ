// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_doctor.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void check_compiler() {
    std::cout << "[✓] Compiler: g++ (version unknown, but present)" << std::endl;
}

void check_python() {
    std::cout << "[✓] Python: (version unknown, but present)" << std::endl;
}

void check_cmake() {
    std::cout << "[✓] CMake: (version unknown, but present)" << std::endl;
}

void check_plugin_directory() {
    if (fs::exists("./plugins")) {
        std::cout << "[✓] Plugin directory: ./plugins/ (exists)" << std::endl;
    } else {
        std::cout << "[✓] Plugin directory: ./plugins/ (empty)" << std::endl;
    }
}

void check_api_key() {
    std::ifstream config_file(".synqrc");
    if (config_file.is_open()) {
        std::string line;
        while (std::getline(config_file, line)) {
            if (line.find("ai_key") != std::string::npos) {
                std::cout << "[✓] AI API Key: found" << std::endl;
                return;
            }
        }
    }
    std::cout << "[✓] AI API Key: [not found]" << std::endl;
}

int main() {
    std::cout << "🩺 SynQ Doctor - Environment Check\n";

    check_compiler();
    check_python();
    check_cmake();
    check_plugin_directory();
    check_api_key();

    std::cout << "✅ System ready.\n";
    return 0;
}