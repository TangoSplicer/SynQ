// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// hover_info.cpp
#include "hover_info.h"
#include <map>
#include <sstream>
#include <vector>

namespace synq::lsp {

  static std::map<std::string, std::string> doc_map = {
    {"let", "Defines a new immutable variable"},
    {"mut", "Defines a mutable variable"},
    {"qapply", "Applies a quantum gate to the target qubit(s)"},
    {"measure", "Collapses a qubit and returns its classical value"}
  };

  std::string extract_token(const std::string& line, int pos) {
    int start = line.find_last_of(" \t", pos);
    int end = line.find_first_of(" \t();", pos);
    return line.substr(start == std::string::npos ? 0 : start + 1,
                       (end == std::string::npos ? line.size() : end) - (start + 1));
  }

  std::string get_hover_info(const std::string& code, int line, int char_pos) {
    std::istringstream iss(code);
    std::string current_line;
    for (int i = 0; i <= line && std::getline(iss, current_line); ++i) {
      if (i == line) {
        std::string token = extract_token(current_line, char_pos);
        auto it = doc_map.find(token);
        return it != doc_map.end() ? it->second : "No documentation found";
      }
    }
    return "";
  }

}