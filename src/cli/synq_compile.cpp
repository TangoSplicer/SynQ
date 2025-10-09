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


// Author: SynQ Contributors
// File: synq_compile.cpp – Compiler frontend + AI lint integration

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void compile_circuit(const std::string& input_file, bool lint_enabled) {
    std::cout << "🛠 Compiling: " << input_file << "\n";

    // Simulate parsing and compilation
    std::string parsed_ir = "build/parsed_ir.json";

    // For demo, assume the IR file is already generated (by parser pipeline)
    if (lint_enabled) {
        std::string cmd = "python3 -c \"import sys; sys.path.append('src/ai/linter'); "
                          "import linter_engine as le; "
                          "import json; "
                          "ir=json.load(open('" + parsed_ir + "')); "
                          "le.print_lint_report(ir)\"";
        std::system(cmd.c_str());
    }

    std::cout << "✅ Compilation complete.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: synq compile <input_file> [--lint]\n";
        return 1;
    }

    std::string input_file = argv[1];
    bool lint_enabled = (argc > 2 && std::string(argv[2]) == "--lint");

    compile_circuit(input_file, lint_enabled);
    return 0;
}