// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_COPILOT_HOOKS_H
#define SYNQ_COPILOT_HOOKS_H

#include <string>
#include <nlohmann/json.hpp>
#include "../compiler/repl.h"

namespace synq {

enum class CopilotMode {
    Ghostwriter,
    ELI5,
    Debugger,
    Hardcore
};

class CopilotHooks {
public:
    explicit CopilotHooks(REPL& replInstance);
    ~CopilotHooks();

    // Main routing entrypoint
    std::string handlePluginInteraction(const std::string& pluginName,
                                        const nlohmann::json& input,
                                        CopilotMode mode);

    // Optional: mode switching
    void setMode(CopilotMode newMode);

private:
    REPL& repl_;
    CopilotMode currentMode_;

    std::string routeGhostwriter(const std::string& name, const nlohmann::json& input);
    std::string routeELI5(const std::string& name, const nlohmann::json& input);
    std::string routeDebugger(const std::string& name, const nlohmann::json& input);
    std::string routeHardcore(const std::string& name, const nlohmann::json& input);
};

} // namespace synq

#endif // SYNQ_COPILOT_HOOKS_H