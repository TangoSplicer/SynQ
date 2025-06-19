// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// ai_inline_edit.h
#pragma once
#include <string>

namespace synq::lsp {

  class InlineEditAI {
  public:
    InlineEditAI();
    std::string suggest_edit(const std::string& code_context, const std::string& cursor_token);
    std::string refactor_code(const std::string& code);
  };

}