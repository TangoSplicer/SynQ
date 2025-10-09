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