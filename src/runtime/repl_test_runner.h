// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_REPL_TEST_RUNNER_H
#define SYNQ_REPL_TEST_RUNNER_H

#include <string>

namespace synq {

class REPLTestRunner {
public:
    static void run(const std::string& code);
};

}

#endif // SYNQ_REPL_TEST_RUNNER_H