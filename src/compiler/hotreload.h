// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// hotreload.h
#pragma once
#include <string>
#include <unordered_map>
#include "runtime.h"
#include "trust_manager.h"

namespace synq::compiler {

  class HotReloader {
  public:
    HotReloader(Runtime* target);

    bool reload_module(const std::string& path);
    void register_module(const std::string& name, const std::string& path);

  private:
    Runtime* runtime;
    TrustManager trust;
    std::unordered_map<std::string, std::string> module_registry;
  };

}