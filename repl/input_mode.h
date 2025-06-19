// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// input_mode.h
#pragma once
#include <string>

namespace synq::repl {

  class InputMode {
  public:
    static bool is_multiline(const std::string& line);
    static bool is_command(const std::string& line);
    static std::string parse_command(const std::string& line);
  };

}