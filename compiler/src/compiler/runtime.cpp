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
// runtime.cpp
#include "runtime.h"
#include "../ui/debugger_overlay.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

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