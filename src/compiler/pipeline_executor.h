// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// pipeline_executor.h
#pragma once
#include <memory>
#include <string>
#include <vector>
#include "../ui/debugger_overlay.h"

namespace synq::compiler {

  struct PipelineStage {
    std::string name;
    std::string type; // "classical", "quantum", "ai"
  };

  struct Pipeline {
    std::vector<PipelineStage> stages;
  };

  class PipelineExecutor {
  public:
    PipelineExecutor();

    void run_pipeline(const Pipeline& pipeline);

  private:
    std::shared_ptr<synq::ui::DebuggerOverlay> overlay;
  };

}