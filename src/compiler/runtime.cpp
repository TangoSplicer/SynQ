// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// runtime.cpp
#include "runtime.h"
#include "../ui/debugger_overlay.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace synq::compiler {

  Runtime::Runtime() 
    : overlay(std::make_shared<synq::ui::DebuggerOverlay>()) {}

  void Runtime::initialize() {
    std::cout << "🔧 Runtime initialization complete.\n";
  }

  void Runtime::execute(const Program& program) {
    std::cout << "🚀 Executing SynQ Program...\n";

    overlay->set_breakpoint("Quantum Bell Pair");
    overlay->set_breakpoint("AI Summary");

    for (const auto& instr : program.instructions) {
      overlay->enter_stage(instr.op);

      if (instr.op == "print") {
        std::cout << "[print] " << instr.args[0] << "\n";
        overlay->log("Output: " + instr.args[0]);
      }
      else if (instr.op == "delay") {
        int duration = std::stoi(instr.args[0]);
        std::cout << "[delay] " << duration << "ms\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(duration));
        overlay->log("Waited: " + instr.args[0] + "ms");
      }
      else if (instr.op == "quantum") {
        std::cout << "[quantum] Invoking quantum kernel...\n";
        overlay->log("Quantum invocation: " + instr.args[0]);
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
      }
      else if (instr.op == "ai") {
        std::cout << "[ai] Querying AI agent...\n";
        overlay->log("AI agent: " + instr.args[0]);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
      }
      else {
        std::cout << "[warn] Unknown instruction: " << instr.op << "\n";
        overlay->log("Unknown op: " + instr.op);
      }

      overlay->render();
    }

    std::cout << "✅ Execution complete.\n";
  }

}