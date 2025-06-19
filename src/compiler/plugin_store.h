// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_PLUGIN_STORE_H
#define SYNQ_PLUGIN_STORE_H

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

namespace synq {

    using PluginFunction = std::function<nlohmann::json(const nlohmann::json&)>;

    struct PluginMetadata {
        std::string name;
        std::string version;
        std::string description;
    };

    class CompilerPlugin {
    public:
        CompilerPlugin(PluginMetadata metadata, PluginFunction handler);

        nlohmann::json invoke(const nlohmann::json& input) const;
        PluginMetadata get_metadata() const;

    private:
        PluginMetadata metadata_;
        PluginFunction handler_;
    };

    class PluginStore {
    public:
        static PluginStore& instance();

        void register_plugin(const std::string& name, CompilerPlugin plugin);
        std::vector<std::string> list_plugins() const;
        std::optional<CompilerPlugin> get_plugin(const std::string& name) const;

    private:
        std::map<std::string, CompilerPlugin> plugins_;
    };

} // namespace synq

#endif  // SYNQ_PLUGIN_STORE_H