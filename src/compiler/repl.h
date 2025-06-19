// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_REPL_H
#define SYNQ_REPL_H

#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

namespace synq {

class REPL {
public:
    REPL();
    ~REPL();

    // Execute any DSL command
    std::string execute(const std::string& command);

    // Plugin-specific entrypoints
    nlohmann::json list_plugins();
    bool register_plugin(const std::string& name, const std::string& version, const std::string& desc, const std::string& handler);
    nlohmann::json invoke_plugin(const std::string& name, const nlohmann::json& payload);

private:
    std::map<std::string, std::pair<nlohmann::json, std::function<nlohmann::json(nlohmann::json)>>> plugin_registry_;
};

} // namespace synq

#endif // SYNQ_REPL_H