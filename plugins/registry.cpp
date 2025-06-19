// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/registry.cpp

#include "registry.h"
#include <iostream>

namespace synq {

void PluginRegistry::registerPlugin(const std::shared_ptr<Plugin>& plugin) {
    std::string id = plugin->name();
    if (loaded.count(id)) {
        std::cout << "[PLUGIN] Already registered: " << id << "\n";
        return;
    }
    plugin->onLoad();
    loaded[id] = plugin;
    std::cout << "[PLUGIN] Loaded: " << id << " v" << plugin->version() << "\n";
}

void PluginRegistry::unloadPlugin(const std::string& name) {
    auto it = loaded.find(name);
    if (it != loaded.end()) {
        it->second->onUnload();
        loaded.erase(it);
        std::cout << "[PLUGIN] Unloaded: " << name << "\n";
    }
}

void PluginRegistry::runPlugin(const std::string& name) {
    if (loaded.count(name)) {
        loaded[name]->run();
    } else {
        std::cerr << "[PLUGIN] Not found: " << name << "\n";
    }
}

bool PluginRegistry::isLoaded(const std::string& name) const {
    return loaded.count(name) > 0;
}

PluginRegistry& PluginRegistry::instance() {
    static PluginRegistry instance;
    return instance;
}

} // namespace synq