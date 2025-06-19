// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// input_mode.cpp
#include "input_mode.h"
#include <algorithm>

namespace synq::repl {

  bool InputMode::is_multiline(const std::string& line) {
    return line.find("{") != std::string::npos || line.find("}") != std::string::npos;
  }

  bool InputMode::is_command(const std::string& line) {
    return line.size() > 1 && line[0] == ':';
  }

  std::string InputMode::parse_command(const std::string& line) {
    std::string cmd = line.substr(1);
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    return cmd;
  }

}