// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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