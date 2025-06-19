// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "repl_test_runner.h"
#include "runtime_engine.h"
#include <iostream>

namespace synq {

void REPLTestRunner::run(const std::string& code) {
    std::cout << "[TestRunner] Running test block...\n";
    RuntimeEngine::execute(code);
}

}