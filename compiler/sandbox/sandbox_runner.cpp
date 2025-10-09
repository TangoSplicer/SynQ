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
// sandbox_runner.cpp
#include "sandbox_runner.h"
#include <chrono>
#include <thread>

namespace synq::sandbox {

  SandboxRunner::SandboxRunner(const PluginPolicy& policy)
    : policy(policy), mem_used(0), elapsed_ms(0), error_msg("") {}

  bool SandboxRunner::run(ASTNode* root) {
    auto start = std::chrono::high_resolution_clock::now();

    std::function<bool(ASTNode*)> exec = [&](ASTNode* node) {
      if (!check_constraints(node)) return false;

      for (auto* child : node->children) {
        if (!exec(child)) return false;
      }

      mem_used += 32;
      return true;
    };

    bool ok = exec(root);
    auto end = std::chrono::high_resolution_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if (elapsed_ms > policy.timeout_ms) {
      error_msg = "Execution exceeded timeout";
      return false;
    }

    return ok;
  }

  bool SandboxRunner::check_constraints(ASTNode* node) {
    if (mem_used > policy.max_memory_kb * 1024) {
      error_msg = "Memory limit exceeded";
      return false;
    }
    if (node->type == "native_call" && !policy.is_allowed(Permission::INVOKE_NATIVE)) {
      error_msg = "Native calls are not permitted";
      return false;
    }
    return true;
  }

  std::string SandboxRunner::get_error() const {
    return error_msg;
  }

}