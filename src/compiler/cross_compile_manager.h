// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// cross_compile_manager.h
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "target_profile.h"
#include "codegen_backend.h"

namespace synq::compiler {

  class CrossCompileManager {
  public:
    CrossCompileManager();
    void register_backend(const std::string& target_id, std::shared_ptr<CodegenBackend> backend);
    std::string compile(const ASTNode& root, const TargetProfile& target);

  private:
    std::map<std::string, std::shared_ptr<CodegenBackend>> backends;
  };

}