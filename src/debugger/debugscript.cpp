// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debugscript.h"
#include <iostream>
#include <sstream>

namespace synq {

std::vector<std::string> DebugScript::scriptLines;

void DebugScript::load(const std::string& script) {
    std::istringstream ss(script);
    std::string line;
    while (std::getline(ss, line)) {
        scriptLines.push_back(line);
    }
}

void DebugScript::run() {
    std::cout << "[DebugScript Executing...]\n";
    for (const auto& line : scriptLines) {
        std::cout << "-> " << line << "\n";
        // Hook for evaluating symbolic conditions or triggers
    }
}

void DebugScript::clear() {
    scriptLines.clear();
}

}