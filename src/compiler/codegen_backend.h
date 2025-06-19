// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// codegen_backend.h
#pragma once
#include <string>
#include "../ast/ast.h"
#include "target_profile.h"

namespace synq::compiler {

  class CodegenBackend {
  public:
    virtual ~CodegenBackend() = default;
    virtual std::string generate(const ASTNode& root, const TargetProfile& profile) = 0;
  };

}