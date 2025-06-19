// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "copilot_hooks.h"
#include <sstream>

namespace synq {

CopilotHooks::CopilotHooks(REPL& replInstance)
    : repl_(replInstance), currentMode_(CopilotMode::Ghostwriter) {}

CopilotHooks::~CopilotHooks() {}

void CopilotHooks::setMode(CopilotMode newMode) {
    currentMode_ = newMode;
}

std::string CopilotHooks::handlePluginInteraction(const std::string& pluginName,
                                                  const nlohmann::json& input,
                                                  CopilotMode mode) {
    switch (mode) {
        case CopilotMode::Ghostwriter:
            return routeGhostwriter(pluginName, input);
        case CopilotMode::ELI5:
            return routeELI5(pluginName, input);
        case CopilotMode::Debugger:
            return routeDebugger(pluginName, input);
        case CopilotMode::Hardcore:
            return routeHardcore(pluginName, input);
        default:
            return "[Error] Unknown mode";
    }
}

std::string CopilotHooks::routeGhostwriter(const std::string& name, const nlohmann::json& input) {
    auto result = repl_.invoke_plugin(name, input);
    std::ostringstream scaffold;
    scaffold << "// Auto-generated plugin call\n";
    scaffold << "// Result: " << result.dump(2) << "\n";
    scaffold << "// Ready for editing in IDE";
    return scaffold.str();
}

std::string CopilotHooks::routeELI5(const std::string& name, const nlohmann::json& input) {
    auto result = repl_.invoke_plugin(name, input);
    std::ostringstream friendly;
    friendly << "📦 Plugin `" << name << "` did this:\n";
    friendly << result.dump(2) << "\n\n";
    friendly << "Think of it like a kitchen robot that chopped and arranged your data.";
    return friendly.str();
}

std::string CopilotHooks::routeDebugger(const std::string& name, const nlohmann::json& input) {
    auto result = repl_.invoke_plugin(name, input);
    std::ostringstream trace;
    trace << "🪲 Plugin Trace Output for: " << name << "\n";
    trace << result.dump(2) << "\n";
    trace << "[Debugger Mode] — Review error keys, stack traces, or output anomalies.\n";
    return trace.str();
}

std::string CopilotHooks::routeHardcore(const std::string& name, const nlohmann::json& input) {
    auto result = repl_.invoke_plugin(name, input);
    std::ostringstream bare;
    bare << result.dump(2) << "\n";
    bare << "[No assistance provided. You’re flying solo ✈️]";
    return bare.str();
}

} // namespace synq