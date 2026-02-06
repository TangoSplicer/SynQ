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


// © 2025 SynQ Contributors. All rights reserved.
// File: synq_export.cpp — CLI interface for SynQ Exporter

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 4 || std::string(argv[1]) != "export") {
        std::cout << "Usage: synq export <source_file.synq> --target <synqvm|wasm|llvm>\n";
        return 1;
    }

    std::string source = argv[2];
    std::string target = argv[4];

    std::string command = "python3 -c \"import sys; sys.path.append('src/compiler'); "
                          "from exporter import export; export('" + source + "', '" + target + "')\"";

    std::system(command.c_str());
    return 0;
}