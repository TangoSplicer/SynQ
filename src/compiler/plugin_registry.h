// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_PLUGIN_REGISTRY_H
#define SYNQ_PLUGIN_REGISTRY_H

#include <string>
#include <vector>
#include <map>

struct PluginInfo {
    std::string name;
    std::string description;
    std::string version;
    std::vector<std::string> tags;
    int usage_count = 0;
    bool is_deprecated = false;
    bool is_experimental = false;
};

class PluginRegistry {
public:
    void registerPlugin(const PluginInfo& plugin);
    PluginInfo* getPlugin(const std::string& name);
    std::vector<PluginInfo> search(const std::string& query);
    void incrementUsage(const std::string& name);
    std::vector<PluginInfo> getAll();

private:
    std::map<std::string, PluginInfo> registry;
};

#endif