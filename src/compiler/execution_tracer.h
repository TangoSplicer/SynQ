// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// execution_tracer.h
#pragma once
#include <string>
#include <vector>
#include <chrono>

namespace synq::compiler {

  struct TraceEntry {
    std::string stage;
    std::string type;
    std::string result;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
  };

  class ExecutionTracer {
  public:
    void start_trace(const std::string& stage, const std::string& type);
    void end_trace(const std::string& result = "");
    void clear();
    std::vector<TraceEntry> get_trace() const;

  private:
    std::vector<TraceEntry> trace_log;
    TraceEntry current;
    bool active = false;
  };

}