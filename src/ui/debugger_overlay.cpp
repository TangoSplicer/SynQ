// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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