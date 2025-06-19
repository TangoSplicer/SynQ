// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "copilot_agent.h"
#include "copilot_hooks.h"
#include "../compiler/repl.h"
#include <iostream>
#include <map>

namespace synq {

// Simulated user preferences or context-aware defaults
static std::map<std::string, CopilotMode> modeMap = {
    {"ghostwriter", CopilotMode::Ghostwriter},
    {"eli5", CopilotMode::ELI5},
    {"debugger", CopilotMode::Debugger},
    {"hardcore", CopilotMode::Hardcore}
};

CopilotAgent::CopilotAgent(REPL& replInstance)
    : repl_(replInstance), hooks_(replInstance), mode_(CopilotMode::Ghostwriter) {}

CopilotAgent::~CopilotAgent() {}

void CopilotAgent::setMode(const std::string& modeStr) {
    if (modeMap.count(modeStr)) {
        mode_ = modeMap[modeStr];
        hooks_.setMode(mode_);
    } else {
        std::cerr << "[Copilot] Unknown mode: " << modeStr << std::endl;
    }
}

std::string CopilotAgent::handleUserIntent(const std::string& pluginName, const nlohmann::json& payload) {
    return hooks_.handlePluginInteraction(pluginName, payload, mode_);
}

std::string CopilotAgent::autoSuggestPlugin(const std::string& userPrompt) {
    // Simple example: future version could use LLM embedding search
    if (userPrompt.find("optimize") != std::string::npos)
        return "qsim_speedup";
    if (userPrompt.find("debug") != std::string::npos)
        return "visual_debugger";
    if (userPrompt.find("expand") != std::string::npos)
        return "ai_expand";

    return "ai_expand"; // default fallback
}

} // namespace synq