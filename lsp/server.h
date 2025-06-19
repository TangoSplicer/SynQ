// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// server.h
#pragma once
#include <string>
#include <map>
#include "symbols.h"
#include "ai_inline_edit.h"

namespace synq::lsp {

  class LSPServer {
  public:
    LSPServer();

    void initialize();
    std::string handle_request(const std::string& json_payload);

  private:
    std::string handle_initialize();
    std::string handle_hover(const std::string& json_payload);
    std::string handle_diagnostics(const std::string& source_code);

    SymbolTable symtab;
    InlineEditAI inline_ai;
  };

}