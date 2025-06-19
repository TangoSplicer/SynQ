// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_COMPILER_AST_H
#define SYNQ_COMPILER_AST_H

#include <vector>
#include <string>

class ASTNode {
public:
    virtual ~ASTNode() {}
};

// AST node for the root of a program (contains a list of statements).
class ProgramNode : public ASTNode {
public:
    std::vector<ASTNode*> statements;
    ~ProgramNode();
};

// (Additional AST node classes like FunctionNode, ClassNode, etc. would be defined here.)

#endif