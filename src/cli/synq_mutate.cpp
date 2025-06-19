// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_mutate.cpp // CLI Entry Point for Plugin Mutation // (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include <iostream> #include <filesystem> #include "../plugins/mutation/mutate_engine.h"

using namespace synq::mutation;

int main(int argc, char* argv[]) { if (argc < 2) { std::cerr << "Usage: synq mutate <plugin_file.synq>\n"; return 1; }

std::string path = argv[1];
if (!std::filesystem::exists(path)) {
    std::cerr << "Error: File not found: " << path << "\n";
    return 1;
}

MutateEngine engine;
MutationResult result = engine.run(path);

std::cout << "===== Mutation Logs =====\n";
for (const auto& log : result.logs) {
    std::cout << log << "\n";
}

std::cout << "\n===== Mutated Plugin Output =====\n";
std::cout << result.mutated_code << "\n";

return 0;

}

