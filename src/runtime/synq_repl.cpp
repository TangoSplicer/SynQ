// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "synq_repl.h"
#include "runtime_engine.h"
#include <iostream>
#include <string>

namespace synq {

bool SynqREPL::running = false;

void SynqREPL::start() {
    running = true;
    std::cout << "Welcome to SynQ REPL. Type `exit` to quit.\n";
    prompt();
}

void SynqREPL::prompt() {
    std::string line;
    while (running) {
        std::cout << ">>> ";
        std::getline(std::cin, line);
        if (line == "exit") {
            running = false;
        } else {
            RuntimeEngine::execute(line);
        }
    }
}

void SynqREPL::stop() {
    running = false;
}

}