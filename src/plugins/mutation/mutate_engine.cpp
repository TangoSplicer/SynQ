// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/plugins/mutation/mutate_engine.cpp
// Mutation Engine Dispatcher
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include "mutate_engine.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace synq {
namespace mutation {

MutateEngine::MutateEngine() {
    oracle = std::make_unique<ai::AIOracle>();
}

MutateEngine::~MutateEngine() = default;

MutationResult MutateEngine::run(const std::string& plugin_path) {
    MutationResult result;

    // Load plugin source
    std::ifstream file(plugin_path);
    if (!file.is_open()) {
        result.logs.push_back("Error: Unable to open plugin file.");
        return result;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Ask AI oracle for mutation strategy
    auto strategy = oracle->generate_strategy(source);
    result.logs.push_back("AI strategy generated: " + strategy.name);

    // Apply passes
    std::string mutated = source;

    if (strategy.use_rename) {
        result.logs.push_back("Applying RenamePass...");
        mutated = RenamePass::apply(mutated);
    }
    if (strategy.use_insert) {
        result.logs.push_back("Applying InsertPass...");
        mutated = InsertPass::apply(mutated);
    }
    if (strategy.use_delete) {
        result.logs.push_back("Applying DeletePass...");
        mutated = DeletePass::apply(mutated);
    }

    result.mutated_code = mutated;
    return result;
}

} // namespace mutation
} // namespace synq
