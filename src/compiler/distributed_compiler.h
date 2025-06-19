// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// distributed_compiler.h
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "compiler_job.h"

namespace synq::compiler {

  struct RemoteWorker {
    std::string id;
    std::string address;
    int port;
    bool available;
  };

  class DistributedCompiler {
  public:
    DistributedCompiler();
    void register_worker(const std::string& id, const std::string& address, int port);
    void submit_job(const CompilerJob& job);
    void run_all();
    std::map<std::string, std::string> get_results();

  private:
    std::vector<RemoteWorker> workers;
    std::vector<CompilerJob> jobs;
    std::map<std::string, std::string> results;

    void dispatch_job(const CompilerJob& job, RemoteWorker& worker);
  };

}