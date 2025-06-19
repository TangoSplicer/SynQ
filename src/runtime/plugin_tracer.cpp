// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "plugin_tracer.h"
#include <iostream>
#include <vector>

namespace synq {

static std::vector<std::string> loadedPlugins;

void PluginTracer::track(const std::string& pluginName) {
    loadedPlugins.push_back(pluginName);
    std::cout << "[PluginTracer] Plugin tracked: " << pluginName << "\n";
}

void PluginTracer::printTrace() {
    std::cout << "[PluginTracer] Plugins loaded this session:\n";
    for (const auto& p : loadedPlugins) {
        std::cout << " - " << p << "\n";
    }
}

}