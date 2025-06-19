// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/plugins/mutation/ai_guidance/ai_oracle.h
// AI Oracle for Mutation Strategy
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#ifndef SYNQ_AI_ORACLE_H
#define SYNQ_AI_ORACLE_H

#include <string>

namespace synq {
namespace ai {

struct MutationStrategy {
    std::string name;
    bool use_rename;
    bool use_insert;
    bool use_delete;
};

class AIOracle {
public:
    AIOracle();
    ~AIOracle();

    MutationStrategy generate_strategy(const std::string& source_code);
};

} // namespace ai
} // namespace synq

#endif // SYNQ_AI_ORACLE_H
