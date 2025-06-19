// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_PLUGIN_MANAGER_H
#define SYNQ_PLUGIN_MANAGER_H

#include <string>
#include <vector>
#include <map>

namespace synq {
namespace plugins {

class PluginManager {
public:
    PluginManager();
    void discoverPlugins();
    void installPlugin(const std::string& name);
    void loadPlugin(const std::string& name);
    void listInstalled();
    void removePlugin(const std::string& name);

private:
    std::string pluginPath;
    std::map<std::string, std::string> installed;
    void updateRegistry();
};

} // namespace plugins
} // namespace synq

#endif // SYNQ_PLUGIN_MANAGER_H