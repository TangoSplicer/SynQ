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
#include "session_scriptor.h"
#include <fstream>
#include <iostream>
#include "eval_context.h"

namespace synq {

void SessionScriptor::save(const std::string& path) {
    std::ofstream out(path);
    out << "# SynQ REPL Session Export\n";
    for (const auto& [var, val] : EvalContext::variables) {
        out << var << " = " << val << "\n";
    }
    out.close();
    std::cout << "[SessionScriptor] Saved session to " << path << "\n";
}

void SessionScriptor::load(const std::string& path) {
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        auto eq = line.find('=');
        if (eq != std::string::npos) {
            std::string var = line.substr(0, eq - 1);
            std::string val = line.substr(eq + 2);
            EvalContext::define(var, val);
        }
    }
    in.close();
    std::cout << "[SessionScriptor] Loaded session from " << path << "\n";
}

}