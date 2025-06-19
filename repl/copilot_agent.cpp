// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// copilot_agent.cpp
#include "copilot_agent.h"
#include "../ai/agent_api.h" // Pretend AI core API

namespace synq::repl {

  std::string CopilotAgent::suggest_fix(const std::string& broken_code) {
    std::string prompt = "Suggest a fix for the following broken SynQ code:\n" + broken_code;
    return synq::ai::call_model("copilot", prompt);
  }

  std::string CopilotAgent::explain_error(const std::string& code) {
    std::string prompt = "Explain what this code does and why it might error:\n" + code;
    return synq::ai::call_model("copilot", prompt);
  }

}