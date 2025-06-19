// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// debug_exporter.cpp
#include "debug_exporter.h"
#include <sstream>
#include <iomanip>
#include <chrono>

namespace synq::compiler {

  std::string time_to_str(std::chrono::steady_clock::time_point t) {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count();
    return std::to_string(ms);
  }

  std::string duration_str(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    return std::to_string(dur) + "ms";
  }

  std::string DebugExporter::to_json(const std::vector<TraceEntry>& trace) {
    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < trace.size(); ++i) {
      const auto& e = trace[i];
      oss << "  {\n";
      oss << "    \"stage\": \"" << e.stage << "\",\n";
      oss << "    \"type\": \"" << e.type << "\",\n";
      oss << "    \"start\": " << time_to_str(e.start) << ",\n";
      oss << "    \"end\": " << time_to_str(e.end) << ",\n";
      oss << "    \"duration\": \"" << duration_str(e.start, e.end) << "\",\n";
      oss << "    \"result\": \"" << e.result << "\"\n";
      oss << "  }" << (i < trace.size() - 1 ? "," : "") << "\n";
    }
    oss << "]";
    return oss.str();
  }

  std::string DebugExporter::to_markdown(const std::vector<TraceEntry>& trace) {
    std::ostringstream oss;
    oss << "| Stage | Type | Duration | Result |\n";
    oss << "|-------|------|----------|--------|\n";
    for (const auto& e : trace) {
      oss << "| " << e.stage << " | "
          << e.type << " | "
          << duration_str(e.start, e.end) << " | "
          << e.result << " |\n";
    }
    return oss.str();
  }

}