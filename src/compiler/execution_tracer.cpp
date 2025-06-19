// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// execution_tracer.cpp
#include "execution_tracer.h"

namespace synq::compiler {

  void ExecutionTracer::start_trace(const std::string& stage, const std::string& type) {
    if (active) end_trace();
    current.stage = stage;
    current.type = type;
    current.start = std::chrono::steady_clock::now();
    active = true;
  }

  void ExecutionTracer::end_trace(const std::string& result) {
    if (!active) return;
    current.end = std::chrono::steady_clock::now();
    current.result = result;
    trace_log.push_back(current);
    active = false;
  }

  void ExecutionTracer::clear() {
    trace_log.clear();
    active = false;
  }

  std::vector<TraceEntry> ExecutionTracer::get_trace() const {
    return trace_log;
  }

}