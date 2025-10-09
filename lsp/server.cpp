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