// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// diagnostics.cpp
#include "diagnostics.h"
#include <regex>

using json = nlohmann::json;

namespace synq::lsp {

  json run_diagnostics(const std::string& source) {
    json results = json::array();
    int line_number = 0;
    std::istringstream stream(source);
    std::string line;

    while (std::getline(stream, line)) {
      if (line.find(";;") != std::string::npos) {
        results.push_back({
          {"range", {
            {"start", {{"line", line_number}, {"character", 0}}},
            {"end",   {{"line", line_number}, {"character", (int)line.length()}}}
          }},
          {"message", "Double semicolon is not valid syntax"},
          {"severity", 2}
        });
      }
      line_number++;
    }

    return results;
  }

}