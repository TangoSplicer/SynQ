// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/plugins/mutation/mutate_test.cpp // Manual test harness for MutateEngine // (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include "mutate_engine.h" #include <iostream>

int main(int argc, char* argv[]) { if (argc < 2) { std::cerr << "Usage: mutate_test <plugin_file.synq>\n"; return 1; }

synq::mutation::MutateEngine engine;
auto result = engine.run(argv[1]);

std::cout << "===== Mutation Logs =====\n";
for (const auto& log : result.logs)
    std::cout << log << std::endl;

std::cout << "\n===== Mutated Code =====\n";
std::cout << result.mutated_code << std::endl;

return 0;

}

