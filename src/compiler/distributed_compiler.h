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