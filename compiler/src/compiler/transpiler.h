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
// File: src/compiler/transpiler.h

#ifndef SYNQ_COMPILER_TRANSPILER_H
#define SYNQ_COMPILER_TRANSPILER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace synq {

enum class TargetFormat {
    QASM,
    CIRQ,
    BRAKET
};

class Transpiler {
public:
    Transpiler();

    void load_ir(const std::string& ir);
    std::string export_to(TargetFormat format);

private:
    std::string ir;
    std::vector<std::string> tokens;

    void tokenize();
    std::string to_qasm();
    std::string to_cirq();
    std::string to_braket();
};

} // namespace synq

#endif // SYNQ_COMPILER_TRANSPILER_H