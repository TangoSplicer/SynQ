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
// File: src/compiler/solver.cpp

#include "solver.h"
#include <sstream>
#include <algorithm>
#include <iostream>

namespace synq {

SolverBridge::SolverBridge() : type(SolverType::CVX) {}

void SolverBridge::set_type(SolverType t) {
    type = t;
}

void SolverBridge::add_variable(const std::string& name, double value) {
    variables[name] = value;
}

void SolverBridge::add_constraint(const std::string& lhs, const std::string& op, const std::string& rhs) {
    constraints.push_back({lhs, op, rhs});
}

void SolverBridge::set_objective(const std::string& expr) {
    objective = expr;
}

std::string SolverBridge::solve() {
    switch (type) {
        case SolverType::SAT: return solve_sat();
        case SolverType::CVX: return solve_cvx();
        case SolverType::ANNEAL: return solve_anneal();
        default: return "Unknown solver type";
    }
}

std::string SolverBridge::solve_sat() {
    std::ostringstream out;
    out << "[SAT Solver] Constraints: " << constraints.size() << "\n";
    for (const auto& c : constraints) {
        out << c.lhs << " " << c.op << " " << c.rhs << "\n";
    }
    return out.str();
}

std::string SolverBridge::solve_cvx() {
    std::ostringstream out;
    out << "[CVX Solver] Minimize: " << objective << "\n";
    for (const auto& c : constraints) {
        out << "s.t. " << c.lhs << " " << c.op << " " << c.rhs << "\n";
    }
    return out.str();
}

std::string SolverBridge::solve_anneal() {
    std::ostringstream out;
    out << "[Annealer] Objective: " << objective << "\n";
    out << "Variables: ";
    for (const auto& v : variables) {
        out << v.first << " ";
    }
    return out.str();
}

} // namespace synq