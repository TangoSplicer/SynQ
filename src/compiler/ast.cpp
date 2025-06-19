// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "ast.h"

ProgramNode::~ProgramNode() {
    for (ASTNode* stmt : statements) {
        delete stmt;
    }
}