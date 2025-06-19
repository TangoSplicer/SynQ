// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// cross_compile_manager.cpp
#include "cross_compile_manager.h"
#include <iostream>

namespace synq::compiler {

  CrossCompileManager::CrossCompileManager() {}

  void CrossCompileManager::register_backend(const std::string& target_id, std::shared_ptr<CodegenBackend> backend) {
    backends[target_id] = backend;
    std::cout << "🧩 Registered backend: " << target_id << "\n";
  }

  std::string CrossCompileManager::compile(const ASTNode& root, const TargetProfile& target) {
    if (backends.find(target.id) == backends.end()) {
      throw std::runtime_error("No backend for target: " + target.id);
    }
    return backends[target.id]->generate(root, target);
  }

}