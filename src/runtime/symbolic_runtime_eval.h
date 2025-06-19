// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SYMBOLIC_RUNTIME_EVAL_H
#define SYNQ_SYMBOLIC_RUNTIME_EVAL_H

#include <string>

namespace synq {

class SymbolicRuntimeEval {
public:
    static std::string eval(const std::string& expr);
};

}

#endif // SYNQ_SYMBOLIC_RUNTIME_EVAL_H