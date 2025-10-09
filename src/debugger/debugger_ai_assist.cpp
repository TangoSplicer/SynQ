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
#include "debugger_ai_assist.h"
#include <iostream>

namespace synq {

void DebuggerAIAssist::explain(const std::string& errorMsg) {
    std::cout << "\n[AI Assist] Analyzing error: \"" << errorMsg << "\"\n";

    // Simulated "friendly" explanation based on common error keywords
    if (errorMsg.find("undefined") != std::string::npos) {
        std::cout << "[AI Assist] Looks like you're referencing a variable that hasn't been declared.\n";
    } else if (errorMsg.find("type") != std::string::npos) {
        std::cout << "[AI Assist] This might be a type mismatch. Did you try to add a string to an integer?\n";
    } else if (errorMsg.find("division") != std::string::npos) {
        std::cout << "[AI Assist] Division by zero! Double-check your denominator.\n";
    } else {
        std::cout << "[AI Assist] Hmm… not sure exactly, but check for syntax or logic issues near the error.\n";
    }

    std::cout << "[AI Assist] Want more help? Try `explain_verbose(error)` in REPL.\n";
}

}