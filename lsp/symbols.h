// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// symbols.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace synq::lsp {

  struct Symbol {
    std::string name;
    std::string kind;
    int line;
    int character;
  };

  class SymbolTable {
  public:
    void index(const std::string& source);
    nlohmann::json get_symbols() const;
    nlohmann::json find_definition(const std::string& token) const;

  private:
    std::vector<Symbol> symbols;
  };

}