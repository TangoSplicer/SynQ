// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// debug_exporter.h
#pragma once
#include "execution_tracer.h"
#include <string>

namespace synq::compiler {

  class DebugExporter {
  public:
    static std::string to_json(const std::vector<TraceEntry>& trace);
    static std::string to_markdown(const std::vector<TraceEntry>& trace);
  };

}