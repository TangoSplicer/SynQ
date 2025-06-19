// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// server.cpp
#include "server.h"
#include "diagnostics.h"
#include "hover_info.h"
#include "symbols.h"
#include "ai_inline_edit.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace synq::lsp {

  LSPServer::LSPServer() {}

  void LSPServer::initialize() {
    // Optional session state init
  }

  std::string LSPServer::handle_request(const std::string& json_payload) {
    auto req = json::parse(json_payload);
    std::string method = req["method"];

    if (method == "initialize") return handle_initialize();
    if (method == "textDocument/hover") return handle_hover(json_payload);
    if (method == "textDocument/publishDiagnostics")
      return handle_diagnostics(req["params"]["text"]);
    if (method == "textDocument/documentSymbol") {
      symtab.index(req["params"]["text"]);
      return symtab.get_symbols().dump();
    }
    if (method == "textDocument/definition") {
      std::string token = req["params"]["token"];
      return symtab.find_definition(token).dump();
    }
    if (method == "textDocument/suggestInlineEdit") {
      std::string text = req["params"]["text"];
      std::string token = req["params"]["token"];
      return json({{"edit", inline_ai.suggest_edit(text, token)}}).dump();
    }
    if (method == "textDocument/refactor") {
      std::string text = req["params"]["text"];
      return json({{"refactored", inline_ai.refactor_code(text)}}).dump();
    }

    return R"({"error": "Unknown request"})";
  }

  std::string LSPServer::handle_initialize() {
    return R"({
      "capabilities": {
        "hoverProvider": true,
        "textDocumentSync": 1,
        "diagnosticProvider": true,
        "documentSymbolProvider": true,
        "definitionProvider": true,
        "codeActionProvider": true
      }
    })";
  }

  std::string LSPServer::handle_hover(const std::string& json_payload) {
    auto req = json::parse(json_payload);
    std::string text = req["params"]["text"];
    int line = req["params"]["position"]["line"];
    int char_pos = req["params"]["position"]["character"];
    std::string hover_text = get_hover_info(text, line, char_pos);
    return json({ {"contents", hover_text} }).dump();
  }

  std::string LSPServer::handle_diagnostics(const std::string& source_code) {
    auto diags = run_diagnostics(source_code);
    return json({ {"diagnostics", diags} }).dump();
  }

}