// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// cloud_trace_uploader.h
#pragma once
#include "../compiler/execution_tracer.h"
#include <string>
#include <vector>

namespace synq::cloud {

  class CloudTraceUploader {
  public:
    static void upload(const std::vector<synq::compiler::TraceEntry>& trace,
                       const std::string& session_id);
  };

}