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
// File: src/compiler/symbolic.cpp

#include "symbolic.h"
#include <sstream>
#include <algorithm>

namespace synq {

SymbolicExpr::SymbolicExpr() {}

SymbolicExpr::SymbolicExpr(const std::string& gate) {
    gates.push_back(gate);
}

SymbolicExpr::SymbolicExpr(const std::vector<std::string>& seq) {
    gates = seq;
}

SymbolicExpr SymbolicExpr::operator*(const SymbolicExpr& other) const {
    std::vector<std::string> combined = gates;
    combined.insert(combined.end(), other.gates.begin(), other.gates.end());
    return SymbolicExpr(combined);
}

std::string SymbolicExpr::to_string() const {
    std::ostringstream oss;
    for (size_t i = 0; i < gates.size(); ++i) {
        oss << gates[i];
        if (i != gates.size() - 1) oss << " * ";
    }
    return oss.str();
}

SymbolicExpr SymbolicExpr::simplify() const {
    std::vector<std::string> simplified = gates;
    for (size_t i = 0; i + 1 < simplified.size(); ++i) {
        if ((simplified[i] == "H" && simplified[i + 1] == "H") ||
            (simplified[i] == "X" && simplified[i + 1] == "X") ||
            (simplified[i] == "Z" && simplified[i + 1] == "Z")) {
            simplified.erase(simplified.begin() + i, simplified.begin() + i + 2);
            i = -1;  // restart
        }
    }
    return SymbolicExpr(simplified);
}

bool SymbolicExpr::equals(const SymbolicExpr& other) const {
    return simplify().gates == other.simplify().gates;
}

// --------- SymbolicAlgebra ---------

SymbolicExpr SymbolicAlgebra::parse(const std::string& expr_str) {
    std::istringstream iss(expr_str);
    std::string token;
    std::vector<std::string> seq;
    while (iss >> token) {
        if (token != "*" && token != "∘") {
            seq.push_back(token);
        }
    }
    return SymbolicExpr(seq);
}

SymbolicExpr SymbolicAlgebra::identity() {
    return SymbolicExpr(std::vector<std::string>());
}

SymbolicExpr SymbolicAlgebra::dagger(const SymbolicExpr& expr) {
    std::vector<std::string> reversed(expr.to_string().size());
    std::vector<std::string> result(expr.to_string().size());
    auto gates = expr.to_string();

    std::istringstream iss(gates);
    std::string g;
    std::vector<std::string> seq;

    while (iss >> g) {
        if (g != "*" && g != "∘") {
            seq.push_back(g);
        }
    }

    std::reverse(seq.begin(), seq.end());
    for (auto& gate : seq) {
        if (gate == "H" || gate == "X" || gate == "Z" || gate == "I") continue;
        else gate += "†";
    }

    return SymbolicExpr(seq);
}

} // namespace synq