// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "syntrace.h"
#include <iostream>

namespace synq {

std::vector<TraceFrame> SynTrace::callStack;

void SynTrace::enter(const std::string& function, const std::string& file, int line) {
    callStack.push_back({function, file, line, {}, ""});
}

void SynTrace::exit() {
    if (!callStack.empty()) callStack.pop_back();
}

void SynTrace::snapshot(const std::string& var, const std::string& val) {
    if (!callStack.empty()) {
        callStack.back().locals[var] = val;
    }
}

void SynTrace::quantumState(const std::string& snapshot) {
    if (!callStack.empty()) {
        callStack.back().quantumStateSnapshot = snapshot;
    }
}

void SynTrace::printTrace() {
    std::cout << "[SynTrace Stack Trace]\n";
    for (const auto& frame : callStack) {
        std::cout << frame.functionName << " @ " << frame.file << ":" << frame.line << "\n";
        for (const auto& [var, val] : frame.locals) {
            std::cout << "  " << var << " = " << val << "\n";
        }
        if (!frame.quantumStateSnapshot.empty()) {
            std::cout << "  [Quantum] " << frame.quantumStateSnapshot << "\n";
        }
    }
}

void SynTrace::clear() {
    callStack.clear();
}

}