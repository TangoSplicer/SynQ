// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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