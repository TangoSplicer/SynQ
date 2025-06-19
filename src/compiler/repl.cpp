// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "repl.h"
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

namespace synq {

REPL::REPL() {}

REPL::~REPL() {}

std::string REPL::execute(const std::string& command) {
    // Placeholder: interpret DSL command (e.g., @macro or @system call)
    if (command == "@macro list_plugins()") {
        return list_plugins().dump(4);
    }
    return "[!] Unknown DSL command";
}

json REPL::list_plugins() {
    json plugins = json::array();
    for (const auto& [name, meta_pair] : plugin_registry_) {
        json meta = meta_pair.first;
        meta["name"] = name;
        plugins.push_back(meta);
    }
    return plugins;
}

bool REPL::register_plugin(const std::string& name, const std::string& version, const std::string& desc, const std::string& handler) {
    if (plugin_registry_.count(name)) return false;
    json meta;
    meta["version"] = version;
    meta["description"] = desc;
    meta["handler"] = handler;
    plugin_registry_[name] = std::make_pair(meta, nullptr); // handler is a stub for now
    return true;
}

json REPL::invoke_plugin(const std::string& name, const json& payload) {
    if (!plugin_registry_.count(name)) {
        return json{{"error", "plugin not found"}};
    }
    auto handler = plugin_registry_[name].second;
    if (!handler) {
        return json{{"error", "handler not defined"}};
    }
    return handler(payload);
}

} // namespace synq