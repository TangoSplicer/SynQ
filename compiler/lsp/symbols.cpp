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