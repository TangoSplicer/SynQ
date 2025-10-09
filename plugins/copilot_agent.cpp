// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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