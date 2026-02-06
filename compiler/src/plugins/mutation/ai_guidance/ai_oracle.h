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
