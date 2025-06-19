// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// debugger_overlay.h
#pragma once
#include <string>
#include <vector>

namespace synq::ui {

  struct DebuggerOverlayState {
    std::string current_stage;
    std::string status;
    std::vector<std::string> repl_log;
    bool breakpoint_hit = false;
  };

  class DebuggerOverlay {
  public:
    DebuggerOverlay();

    void enter_stage(const std::string& stage_name);
    void log(const std::string& message);
    void set_breakpoint(const std::string& stage);
    void activate_repl();
    void render();

  private:
    DebuggerOverlayState state;
    std::vector<std::string> breakpoints;
  };

}