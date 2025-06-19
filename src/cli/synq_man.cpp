// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_man.cpp

#include <iostream>
#include <string>

void show_doc(const std::string& cmd) {
    if (cmd == "mutate") {
        std::cout << "synq mutate <plugin_dir>\nAI-guided plugin evolution based on performance metrics.\n";
    } else if (cmd == "fuzz") {
        std::cout << "synq fuzz <model>\nRun adversarial prompts to test model stability.\n";
    } else {
        std::cout << "No manual entry for command: " << cmd << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq man <command>\n";
        return 1;
    }

    show_doc(argv[1]);
    return 0;
}