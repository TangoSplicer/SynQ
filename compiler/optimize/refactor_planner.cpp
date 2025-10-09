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