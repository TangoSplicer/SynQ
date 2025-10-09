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
// File: src/compiler/solver.h

#ifndef SYNQ_COMPILER_SOLVER_H
#define SYNQ_COMPILER_SOLVER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace synq {

enum class SolverType {
    SAT,
    CVX,
    ANNEAL
};

struct Constraint {
    std::string lhs;
    std::string op;
    std::string rhs;
};

class SolverBridge {
public:
    SolverBridge();

    void set_type(SolverType type);
    void add_variable(const std::string& name, double value = 0.0);
    void add_constraint(const std::string& lhs, const std::string& op, const std::string& rhs);
    void set_objective(const std::string& expr);
    std::string solve();

private:
    SolverType type;
    std::unordered_map<std::string, double> variables;
    std::vector<Constraint> constraints;
    std::string objective;

    std::string solve_sat();
    std::string solve_cvx();
    std::string solve_anneal();
};

} // namespace synq

#endif // SYNQ_COMPILER_SOLVER_H