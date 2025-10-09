// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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