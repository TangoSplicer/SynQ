// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// refactor_planner.h
#pragma once
#include "../ast.h"

namespace synq::optimize {
  class RefactorPlanner {
  public:
    ASTNode* inline_macros(ASTNode* root);
    ASTNode* fuse_similar_blocks(ASTNode* root);
    ASTNode* remove_dead_code(ASTNode* root);
  };
}