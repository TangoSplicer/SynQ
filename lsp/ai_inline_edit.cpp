// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// ai_inline_edit.cpp
#include "ai_inline_edit.h"
#include "../ai/agent_api.h"

namespace synq::lsp {

  InlineEditAI::InlineEditAI() {}

  std::string InlineEditAI::suggest_edit(const std::string& code_context, const std::string& cursor_token) {
    std::string prompt =
      "You are SynQ AI Assist. The user is editing the following code:\n\n" +
      code_context +
      "\n\nThey are focused on token: '" + cursor_token +
      "'. Suggest a clean inline refactor or improvement.";
    return synq::ai::call_model("refactor", prompt);
  }

  std::string InlineEditAI::refactor_code(const std::string& code) {
    std::string prompt =
      "Refactor the following SynQ code for clarity, performance, and quantum-classical balance:\n\n" + code;
    return synq::ai::call_model("refactor", prompt);
  }

}