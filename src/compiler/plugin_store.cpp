// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "plugin_store.h"
#include <stdexcept>

namespace synq {

    CompilerPlugin::CompilerPlugin(PluginMetadata metadata, PluginFunction handler)
        : metadata_(std::move(metadata)), handler_(std::move(handler)) {}

    nlohmann::json CompilerPlugin::invoke(const nlohmann::json& input) const {
        return handler_(input);
    }

    PluginMetadata CompilerPlugin::get_metadata() const {
        return metadata_;
    }

    PluginStore& PluginStore::instance() {
        static PluginStore instance;
        return instance;
    }

    void PluginStore::register_plugin(const std::string& name, CompilerPlugin plugin) {
        plugins_[name] = std::move(plugin);
    }

    std::vector<std::string> PluginStore::list_plugins() const {
        std::vector<std::string> keys;
        for (const auto& [key, _] : plugins_) {
            keys.push_back(key);
        }
        return keys;
    }

    std::optional<CompilerPlugin> PluginStore::get_plugin(const std::string& name) const {
        auto it = plugins_.find(name);
        if (it != plugins_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

} // namespace synq