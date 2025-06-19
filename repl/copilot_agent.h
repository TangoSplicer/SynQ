// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// copilot_agent.h
#pragma once
#include <string>

namespace synq::repl {

  class CopilotAgent {
  public:
    std::string suggest_fix(const std::string& broken_code);
    std::string explain_error(const std::string& code);
  };

}