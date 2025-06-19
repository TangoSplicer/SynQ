// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// symbols.cpp
#include "symbols.h"
#include <sstream>
#include <regex>

using json = nlohmann::json;

namespace synq::lsp {

  void SymbolTable::index(const std::string& source) {
    symbols.clear();
    std::istringstream stream(source);
    std::string line;
    int lineno = 0;

    std::regex decl_regex(R"((let|mut)\s+([a-zA-Z_][a-zA-Z0-9_]*))");
    std::smatch match;

    while (std::getline(stream, line)) {
      if (std::regex_search(line, match, decl_regex)) {
        Symbol sym;
        sym.kind = "Variable";
        sym.name = match[2];
        sym.line = lineno;
        sym.character = (int)match.position(2);
        symbols.push_back(sym);
      }
      ++lineno;
    }
  }

  json SymbolTable::get_symbols() const {
    json out = json::array();
    for (const auto& sym : symbols) {
      out.push_back({
        {"name", sym.name},
        {"kind", sym.kind},
        {"location", {
          {"line", sym.line},
          {"character", sym.character}
        }}
      });
    }
    return out;
  }

  json SymbolTable::find_definition(const std::string& token) const {
    for (const auto& sym : symbols) {
      if (sym.name == token) {
        return {
          {"line", sym.line},
          {"character", sym.character}
        };
      }
    }
    return nullptr;
  }

}