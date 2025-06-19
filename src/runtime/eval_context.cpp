// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "eval_context.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> EvalContext::variables;

void EvalContext::define(const std::string& var, const std::string& val) {
    variables[var] = val;
}

std::string EvalContext::resolve(const std::string& var) {
    return variables.count(var) ? variables[var] : "[undefined]";
}

void EvalContext::dump() {
    std::cout << "[EvalContext Dump]\n";
    for (const auto& [k, v] : variables) {
        std::cout << "  " << k << " = " << v << "\n";
    }
}

void EvalContext::clear() {
    variables.clear();
}

}