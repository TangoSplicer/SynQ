// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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