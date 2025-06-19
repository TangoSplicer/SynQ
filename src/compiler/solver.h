// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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