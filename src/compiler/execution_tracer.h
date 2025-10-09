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