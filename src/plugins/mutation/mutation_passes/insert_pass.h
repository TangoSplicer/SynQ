// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/plugins/mutation/mutation_passes/insert_pass.h
// InsertPass - Code Injection Transformer
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#ifndef SYNQ_INSERT_PASS_H
#define SYNQ_INSERT_PASS_H

#include <string>

namespace synq {
namespace mutation {

class InsertPass {
public:
    static std::string apply(const std::string& source);
};

} // namespace mutation
} // namespace synq

#endif // SYNQ_INSERT_PASS_H
