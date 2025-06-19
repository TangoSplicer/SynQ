// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// refactor_planner.cpp
#include "refactor_planner.h"
#include <set>

namespace synq::optimize {

  ASTNode* RefactorPlanner::inline_macros(ASTNode* root) {
    if (!root) return nullptr;
    for (auto*& child : root->children) {
      if (child->type == "macro_call") {
        child->type = "inlined_code"; // simplified
        child->value = "/* inlined */";
      }
      inline_macros(child);
    }
    return root;
  }

  ASTNode* RefactorPlanner::fuse_similar_blocks(ASTNode* root) {
    if (!root) return nullptr;
    std::map<std::string, int> counts;
    for (auto* child : root->children) {
      counts[child->type]++;
    }
    if (counts["compute"] >= 2) {
      ASTNode* fused = new ASTNode{"compute_fused", "", {}};
      root->children.push_back(fused);
    }
    for (auto*& child : root->children) {
      fuse_similar_blocks(child);
    }
    return root;
  }

  ASTNode* RefactorPlanner::remove_dead_code(ASTNode* root) {
    if (!root) return nullptr;
    std::vector<ASTNode*> filtered;
    for (auto* child : root->children) {
      if (child->type != "noop" && child->type != "unreachable") {
        filtered.push_back(child);
      }
    }
    root->children = filtered;
    for (auto*& child : root->children) {
      remove_dead_code(child);
    }
    return root;
  }

}