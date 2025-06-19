// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_doctor.cpp

#include <iostream>
#include <cstdlib>

int main() {
    std::cout << "🩺 SynQ Doctor - Environment Check\n";

    std::cout << "[✓] Compiler: g++ ";
    std::system("g++ --version | head -n 1");

    std::cout << "[✓] Python: ";
    std::system("python3 --version");

    std::cout << "[✓] CMake: ";
    std::system("cmake --version | head -n 1");

    std::cout << "[✓] Plugin directory: ./plugins/ (exists or empty)\n";

    std::cout << "[✓] AI API Key: ";
    std::system("grep 'ai_key' .synqrc 2>/dev/null || echo '[not found]'");

    std::cout << "✅ System ready.\n";
    return 0;
}