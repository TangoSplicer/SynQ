// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// cloud_trace_uploader.cpp
#include "cloud_trace_uploader.h"
#include "../compiler/debug_exporter.h"
#include <iostream>
#include <fstream>

namespace synq::cloud {

  void CloudTraceUploader::upload(const std::vector<synq::compiler::TraceEntry>& trace,
                                  const std::string& session_id) {
    std::string json = synq::compiler::DebugExporter::to_json(trace);
    std::string filename = "trace_" + session_id + ".json";
    std::ofstream out(filename);
    out << json;
    out.close();

    // Simulated cloud push
    std::cout << "☁️ Trace uploaded as `" << filename << "` (local simulation)\n";
  }

}