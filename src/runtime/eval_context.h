// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_EVAL_CONTEXT_H
#define SYNQ_EVAL_CONTEXT_H

#include <string>
#include <map>

namespace synq {

class EvalContext {
public:
    static void define(const std::string& var, const std::string& val);
    static std::string resolve(const std::string& var);
    static void dump();
    static void clear();

private:
    static std::map<std::string, std::string> variables;
};

}

#endif // SYNQ_EVAL_CONTEXT_H