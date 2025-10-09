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