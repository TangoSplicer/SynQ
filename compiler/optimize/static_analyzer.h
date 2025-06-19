// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// static_analyzer.h
#pragma once
#include <string>
#include <map>
#include "../ast.h"

namespace synq::optimize {
  struct AnalysisResult {
    int estimated_time_ms;
    int memory_bytes;
    int symbolic_cost;
    std::map<std::string, int> node_weights;
  };

  class StaticAnalyzer {
  public:
    AnalysisResult analyze(ASTNode* root);
  };
}