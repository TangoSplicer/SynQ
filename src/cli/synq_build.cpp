// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_build.cpp

#include <iostream>
#include <cstdlib>
#include <filesystem>

int main(int argc, char* argv[]) {
    std::cout << "🔧 Building SynQ project...\n";

    std::string build_dir = "./build";
    if (!std::filesystem::exists(build_dir)) {
        std::filesystem::create_directory(build_dir);
    }

    std::string cmd = "cd build && cmake .. && make -j$(nproc)";
    int result = std::system(cmd.c_str());

    if (result != 0) {
        std::cerr << "❌ Build failed.\n";
        return 1;
    }

    std::cout << "✅ Build complete.\n";
    return 0;
}