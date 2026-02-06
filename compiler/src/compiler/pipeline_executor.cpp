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
// pipeline_executor.cpp
#include "pipeline_executor.h"
#include "../ui/debugger_overlay.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace synq::compiler {

  PipelineExecutor::PipelineExecutor()
    : overlay(std::make_shared<synq::ui::DebuggerOverlay>()) {}

  void PipelineExecutor::run_pipeline(const Pipeline& pipeline) {
    std::cout << "🔁 Executing pipeline...\n";

    overlay->set_breakpoint("Quantum Bell Pair");
    overlay->set_breakpoint("AI Summary");

    for (const auto& stage : pipeline.stages) {
      overlay->enter_stage(stage.name);

      if (stage.type == "classical") {
        std::cout << "[classical] " << stage.name << "\n";
        overlay->log("Classical step: " + stage.name);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
      else if (stage.type == "quantum") {
        std::cout << "[quantum] " << stage.name << "\n";
        overlay->log("Quantum execution: " + stage.name);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
      }
      else if (stage.type == "ai") {
        std::cout << "[ai] " << stage.name << "\n";
        overlay->log("AI action: " + stage.name);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
      }
      else {
        std::cout << "[warn] Unknown stage type: " << stage.type << "\n";
        overlay->log("Unknown stage type: " + stage.type);
      }

      overlay->render();
    }

    std::cout << "✅ Pipeline execution complete.\n";
  }

}