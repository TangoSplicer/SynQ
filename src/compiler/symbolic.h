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
// File: src/compiler/symbolic.h

#ifndef SYNQ_COMPILER_SYMBOLIC_H
#define SYNQ_COMPILER_SYMBOLIC_H

#include <string>
#include <vector>
#include <unordered_map>

namespace synq {

class SymbolicExpr {
public:
    SymbolicExpr();
    SymbolicExpr(const std::string& gate);
    SymbolicExpr operator*(const SymbolicExpr& other) const;

    std::string to_string() const;
    SymbolicExpr simplify() const;
    bool equals(const SymbolicExpr& other) const;

private:
    std::vector<std::string> gates;

    SymbolicExpr(const std::vector<std::string>& gates);
};

class SymbolicAlgebra {
public:
    static SymbolicExpr parse(const std::string& expr_str);
    static SymbolicExpr identity();
    static SymbolicExpr dagger(const SymbolicExpr& expr);
};

} // namespace synq

#endif // SYNQ_COMPILER_SYMBOLIC_H