// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_config.cpp

#include <iostream>
#include <fstream>
#include <string>

std::string config_file = ".synqrc";

void set_config(const std::string& key, const std::string& value) {
    std::ofstream config(config_file, std::ios::app);
    if (config) {
        config << "\"" << key << "\": \"" << value << "\",\n";
        config.close();
        std::cout << "✅ Config set: " << key << " = " << value << "\n";
    } else {
        std::cerr << "❌ Failed to write config.\n";
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: synq config <key> <value>\n";
        std::cerr << "  --config-file <path>\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--config-file" && i + 1 < argc) {
            config_file = argv[++i];
        }
    }

    set_config(argv[1], argv[2]);
    return 0;
}