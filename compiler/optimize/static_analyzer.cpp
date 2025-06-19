// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// static_analyzer.cpp
#include "static_analyzer.h"
#include <cmath>

namespace synq::optimize {

  AnalysisResult StaticAnalyzer::analyze(ASTNode* root) {
    AnalysisResult result = {0, 0, 0, {}};

    std::function<void(ASTNode*)> visit = [&](ASTNode* node) {
      if (!node) return;

      std::string type = node->type;
      result.node_weights[type] += 1;

      if (type == "loop") {
        result.estimated_time_ms += 50;
        result.symbolic_cost += 2;
      } else if (type == "quantum_op") {
        result.estimated_time_ms += 100;
        result.memory_bytes += 256;
        result.symbolic_cost += 5;
      } else {
        result.estimated_time_ms += 5;
        result.memory_bytes += 32;
        result.symbolic_cost += 1;
      }

      for (auto* child : node->children) {
        visit(child);
      }
    };

    visit(root);
    return result;
  }

}