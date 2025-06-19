// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "plugin_registry.h"
#include <algorithm>

void PluginRegistry::registerPlugin(const PluginInfo& plugin) {
    registry[plugin.name] = plugin;
}

PluginInfo* PluginRegistry::getPlugin(const std::string& name) {
    auto it = registry.find(name);
    if (it != registry.end()) return &it->second;
    return nullptr;
}

std::vector<PluginInfo> PluginRegistry::search(const std::string& query) {
    std::vector<PluginInfo> results;
    for (const auto& [name, info] : registry) {
        if (name.find(query) != std::string::npos ||
            info.description.find(query) != std::string::npos ||
            std::any_of(info.tags.begin(), info.tags.end(), [&](const std::string& tag) {
                return tag.find(query) != std::string::npos;
            })) {
            results.push_back(info);
        }
    }
    return results;
}

void PluginRegistry::incrementUsage(const std::string& name) {
    if (registry.count(name)) {
        registry[name].usage_count++;
    }
}

std::vector<PluginInfo> PluginRegistry::getAll() {
    std::vector<PluginInfo> all;
    for (const auto& [_, info] : registry) {
        all.push_back(info);
    }
    return all;
}