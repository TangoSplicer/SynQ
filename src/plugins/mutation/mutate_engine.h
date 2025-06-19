// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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
