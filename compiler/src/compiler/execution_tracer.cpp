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