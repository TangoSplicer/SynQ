// Copyright (c) 2025 SynQ Contributors
//
// Compiler-neutral shape validation for the bounded typed quantum AST profile.
#ifndef SYNQ_COMPILER_GATE_VALIDATION_H
#define SYNQ_COMPILER_GATE_VALIDATION_H

#include <optional>
#include <string>

#include "ast.h"

namespace synq::compiler {

struct GateShapeValidationError {
    std::string code;
    std::string message;
    std::string help;
};

// Returns an error only for known recovery-profile gate shapes. Unknown gate
// names are intentionally preserved for future language extensions and remain
// the responsibility of a selected backend to reject or support.
std::optional<GateShapeValidationError> validate_quantum_gate_shape(const QuantumGateNode& gate);

}  // namespace synq::compiler

#endif
