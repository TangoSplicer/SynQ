// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// engine.h
#pragma once
#include <string>
#include <vector>

namespace synq::repl {

  enum class ExecutionMode {
    CLASSICAL,
    QUANTUM,
    HYBRID,
    AI
  };

  struct REPLResult {
    std::string output;
    std::string mode;
    bool success;
  };

  class REPL {
  public:
    REPL();

    void set_mode(ExecutionMode mode);
    ExecutionMode get_mode() const;

    std::string read_line();
    std::string buffer() const;
    void reset_buffer();
    void append_to_buffer(const std::string& line);

    REPLResult evaluate();

  private:
    ExecutionMode mode;
    std::string line_buffer;
  };

}