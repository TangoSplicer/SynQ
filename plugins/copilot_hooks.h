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