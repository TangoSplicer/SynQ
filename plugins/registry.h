// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/registry.h
#pragma once

#include <string>
#include <map>
#include <memory>
#include "plugin.h"

namespace synq {

class PluginRegistry {
private:
    std::map<std::string, std::shared_ptr<Plugin>> loaded;

public:
    void registerPlugin(const std::shared_ptr<Plugin>& plugin);
    void unloadPlugin(const std::string& name);
    void runPlugin(const std::string& name);
    bool isLoaded(const std::string& name) const;

    template<typename T>
    std::shared_ptr<T> get(const std::string& name) const {
        auto it = loaded.find(name);
        if (it != loaded.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    static PluginRegistry& instance(); // singleton
};

} // namespace synq