// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// sandbox_runner.h
#pragma once
#include "../ast.h"
#include "plugin_policy.h"

namespace synq::sandbox {

  class SandboxRunner {
  public:
    SandboxRunner(const PluginPolicy& policy);
    bool run(ASTNode* root);
    std::string get_error() const;
  private:
    PluginPolicy policy;
    std::string error_msg;
    int mem_used;
    int elapsed_ms;

    bool check_constraints(ASTNode* node);
  };
}