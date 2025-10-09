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
// distributed_compiler.cpp
#include "distributed_compiler.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace synq::compiler {

  DistributedCompiler::DistributedCompiler() {}

  void DistributedCompiler::register_worker(const std::string& id, const std::string& address, int port) {
    workers.push_back({id, address, port, true});
    std::cout << "🔗 Registered worker: " << id << " at " << address << ":" << port << "\n";
  }

  void DistributedCompiler::submit_job(const CompilerJob& job) {
    jobs.push_back(job);
  }

  void DistributedCompiler::run_all() {
    std::cout << "🚀 Launching distributed compilation for " << jobs.size() << " job(s)\n";
    size_t index = 0;
    for (auto& job : jobs) {
      if (workers.empty()) {
        std::cerr << "❌ No workers available\n";
        return;
      }
      auto& worker = workers[index % workers.size()];
      dispatch_job(job, worker);
      ++index;
    }
  }

  void DistributedCompiler::dispatch_job(const CompilerJob& job, RemoteWorker& worker) {
    std::cout << "📤 Dispatching job `" << job.name << "` to worker `" << worker.id << "`...\n";
    // Simulated remote job handling
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::string result = "compiled_" + job.name;
    results[job.name] = result;
    std::cout << "✅ Job `" << job.name << "` complete (result: " << result << ")\n";
  }

  std::map<std::string, std::string> DistributedCompiler::get_results() {
    return results;
  }

}