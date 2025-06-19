// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "live_mutator.h"
#include "eval_context.h"
#include <iostream>
#include <map>

namespace synq {

static std::map<std::string, std::string> patchedFunctions;

void LiveMutator::patchVariable(const std::string& var, const std::string& val) {
    EvalContext::define(var, val);
    std::cout << "[LiveMutator] Variable patched: " << var << " = " << val << "\n";
}

void LiveMutator::patchFunction(const std::string& name, const std::string& code) {
    patchedFunctions[name] = code;
    std::cout << "[LiveMutator] Function '" << name << "' was redefined.\n";
}

}