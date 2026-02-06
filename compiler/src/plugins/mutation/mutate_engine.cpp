// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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
