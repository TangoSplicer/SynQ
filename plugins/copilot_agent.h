// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_COPILOT_AGENT_H
#define SYNQ_COPILOT_AGENT_H

#include <string>
#include <vector>

namespace synq {
namespace plugins {

class CopilotAgent {
public:
    CopilotAgent();
    void startSession();
    void respondToPrompt(const std::string& prompt);
    void suggestNextStep();
    void explainCode(const std::string& code);
    void toggleModes(const std::string& mode);
    void rememberContext(const std::string& snippet);

private:
    std::string mode;
    std::vector<std::string> memory;
};

} // namespace plugins
} // namespace synq

#endif // SYNQ_COPILOT_AGENT_H