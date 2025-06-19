// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// plugin_policy.cpp
#include "plugin_policy.h"

namespace synq::sandbox {

  bool PluginPolicy::is_allowed(Permission perm) const {
    return std::find(allowed.begin(), allowed.end(), perm) != allowed.end();
  }

  void PolicyRegistry::register_policy(const PluginPolicy& policy) {
    policies[policy.plugin_id] = policy;
  }

  PluginPolicy PolicyRegistry::get_policy(const std::string& plugin_id) const {
    auto it = policies.find(plugin_id);
    if (it != policies.end()) return it->second;
    throw std::runtime_error("Plugin policy not found: " + plugin_id);
  }

}