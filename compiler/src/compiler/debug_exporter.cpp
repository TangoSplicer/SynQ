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