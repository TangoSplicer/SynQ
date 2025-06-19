// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// engine.cpp
#include "engine.h"
#include "../compiler/runtime.h"
#include "../compiler/symbolic.h"
#include "../compiler/codegen.h"
#include <iostream>

namespace synq::repl {

  REPL::REPL() : mode(ExecutionMode::CLASSICAL), line_buffer("") {}

  void REPL::set_mode(ExecutionMode new_mode) {
    mode = new_mode;
  }

  ExecutionMode REPL::get_mode() const {
    return mode;
  }

  std::string REPL::read_line() {
    std::string input;
    std::getline(std::cin, input);
    return input;
  }

  void REPL::append_to_buffer(const std::string& line) {
    line_buffer += line + "\n";
  }

  void REPL::reset_buffer() {
    line_buffer.clear();
  }

  std::string REPL::buffer() const {
    return line_buffer;
  }

  REPLResult REPL::evaluate() {
    REPLResult result;
    result.mode = (mode == ExecutionMode::CLASSICAL ? "classical" :
                   mode == ExecutionMode::QUANTUM ? "quantum" :
                   mode == ExecutionMode::AI ? "ai" : "hybrid");

    try {
      if (mode == ExecutionMode::CLASSICAL) {
        result.output = synq::runtime::execute(line_buffer);
      } else if (mode == ExecutionMode::QUANTUM) {
        result.output = synq::symbolic::simulate(line_buffer);
      } else if (mode == ExecutionMode::AI) {
        result.output = "AI: " + synq::runtime::copilot_suggest(line_buffer);
      } else {
        result.output = synq::runtime::hybrid_eval(line_buffer);
      }
      result.success = true;
    } catch (const std::exception& e) {
      result.output = std::string("Error: ") + e.what();
      result.success = false;
    }

    reset_buffer();
    return result;
  }

}