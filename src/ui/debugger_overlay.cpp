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
// debugger_overlay.cpp
#include "debugger_overlay.h"
#include <iostream>

namespace synq::ui {

  DebuggerOverlay::DebuggerOverlay() {}

  void DebuggerOverlay::enter_stage(const std::string& stage_name) {
    state.current_stage = stage_name;
    state.status = "running";
    state.breakpoint_hit = false;

    for (const auto& bp : breakpoints) {
      if (bp == stage_name) {
        state.breakpoint_hit = true;
        state.status = "paused";
        std::cout << "🛑 [UI] Breakpoint hit at: " << stage_name << std::endl;
        activate_repl();
        break;
      }
    }
  }

  void DebuggerOverlay::log(const std::string& message) {
    state.repl_log.push_back(message);
  }

  void DebuggerOverlay::set_breakpoint(const std::string& stage) {
    breakpoints.push_back(stage);
  }

  void DebuggerOverlay::activate_repl() {
    std::cout << "[UI::REPL] Type 'continue' to resume execution.\n";
    std::string cmd;
    while (true) {
      std::cout << ">> ";
      std::getline(std::cin, cmd);
      if (cmd == "continue") break;
      std::cout << "[echo] " << cmd << std::endl;
    }
    state.status = "resumed";
  }

  void DebuggerOverlay::render() {
    std::cout << "🧠 Debugger Overlay\n";
    std::cout << "Stage: " << state.current_stage << " | Status: " << state.status << std::endl;
    std::cout << "Logs:\n";
    for (const auto& entry : state.repl_log) {
      std::cout << "  - " << entry << std::endl;
    }
    std::cout << "Breakpoints:\n";
    for (const auto& bp : breakpoints) {
      std::cout << "  • " << bp << std::endl;
    }
    std::cout << "─────────────\n";
  }

}