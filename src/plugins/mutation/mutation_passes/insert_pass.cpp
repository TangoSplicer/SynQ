// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/plugins/mutation/mutation_passes/insert_pass.cpp
// InsertPass - Adds mutation hooks or logs
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include "insert_pass.h"
#include <regex>

namespace synq {
namespace mutation {

std::string InsertPass::apply(const std::string& source) {
    std::string result = source;
    std::regex pattern("int main\(.*\) *\{");
    result = std::regex_replace(result, pattern,
        "int main() {\n    // [Mutation] Inserted log\n    std::cout << \"[Mutation] Starting main\\n\";\n");
    return result;
}

} // namespace mutation
} // namespace synq
