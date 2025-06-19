// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_config.cpp

#include <iostream>
#include <fstream>
#include <string>

const std::string CONFIG_FILE = ".synqrc";

void set_config(const std::string& key, const std::string& value) {
    std::ofstream config(CONFIG_FILE, std::ios::app);
    if (config) {
        config << "\"" << key << "\": \"" << value << "\",\n";
        config.close();
        std::cout << "✅ Config set: " << key << " = " << value << "\n";
    } else {
        std::cerr << "❌ Failed to write config.\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: synq config <key> <value>\n";
        return 1;
    }

    set_config(argv[1], argv[2]);
    return 0;
}