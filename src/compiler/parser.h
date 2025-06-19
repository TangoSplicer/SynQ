// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_COMPILER_PARSER_H
#define SYNQ_COMPILER_PARSER_H

#include <string>
#include "ast.h"

class Parser {
public:
    // Parse a source file and return the AST root node (or nullptr on error).
    ASTNode* parseFile(const std::string& filename);
};

#endif