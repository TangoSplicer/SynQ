// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_COMPILER_CODEGEN_H
#define SYNQ_COMPILER_CODEGEN_H

#include <iostream>
#include "ast.h"

// Representation of compiled program (e.g., bytecode or IR).
struct IRProgram {
    // Placeholder for IR data (e.g., instruction list, symbol table, etc.).
};

class CodeGenerator {
public:
    // Generate an IRProgram from the AST. Returns a pointer to a new IRProgram.
    IRProgram* generate(ASTNode* ast);
};

#endif