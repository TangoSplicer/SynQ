// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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