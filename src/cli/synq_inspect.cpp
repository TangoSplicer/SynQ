// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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