// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// plugin_policy.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace synq::sandbox {

  enum class Permission {
    READ_STD_LIB,
    WRITE_LOCAL,
    INVOKE_NATIVE,
    ACCESS_NETWORK,
    SPAWN_THREADS
  };

  struct PluginPolicy {
    std::string plugin_id;
    std::vector<Permission> allowed;
    int max_memory_kb;
    int timeout_ms;
    bool allow_external_ai;

    bool is_allowed(Permission perm) const;
  };

  class PolicyRegistry {
  public:
    void register_policy(const PluginPolicy& policy);
    PluginPolicy get_policy(const std::string& plugin_id) const;
  private:
    std::unordered_map<std::string, PluginPolicy> policies;
  };
}