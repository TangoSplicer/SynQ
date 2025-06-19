// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "session_macros.h"
#include "eval_context.h"
#include <iostream>

namespace synq {

bool SessionMacros::handle(const std::string& line) {
    if (line == "!clear") {
        EvalContext::clear();
        std::cout << "[Macros] Cleared context.\n";
        return true;
    } else if (line == "!dump") {
        EvalContext::dump();
        return true;
    }
    return false;
}

}