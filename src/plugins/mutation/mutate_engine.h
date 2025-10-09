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
// synq/src/plugins/mutation/mutate_engine.h
// Mutation Engine - Dispatcher Interface
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#ifndef SYNQ_MUTATE_ENGINE_H
#define SYNQ_MUTATE_ENGINE_H

#include <string>
#include <vector>
#include <memory>
#include "ai_guidance/ai_oracle.h"
#include "mutation_passes/rename_pass.h"
#include "mutation_passes/insert_pass.h"
#include "mutation_passes/delete_pass.h"

namespace synq {
namespace mutation {

struct MutationResult {
    std::string mutated_code;
    std::vector<std::string> logs;
};

class MutateEngine {
public:
    MutateEngine();
    ~MutateEngine();

    MutationResult run(const std::string& plugin_path);

private:
    std::unique_ptr<ai::AIOracle> oracle;
};

} // namespace mutation
} // namespace synq

#endif // SYNQ_MUTATE_ENGINE_H
