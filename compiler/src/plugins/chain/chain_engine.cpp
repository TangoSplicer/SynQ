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

// synq/src/plugins/chain/chain_engine.cpp
// Core Execution Engine for SynQ Chain Pipelines
// License: 

#include "chain_engine.h"
#include "graph_parser.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

namespace synq::chain {

ChainEngine::ChainEngine() {}
ChainEngine::~ChainEngine() {}

bool ChainEngine::load_from_file(const std::string& filepath) {
    task_map = parse_chain_graph(filepath);
    return !task_map.empty();
}

bool ChainEngine::resolve_dependencies(const ChainTask& task) {
    for (const auto& dep : task.depends_on) {
        if (task_status.find(dep) == task_status.end() || !task_status[dep]) {
            report("Dependency failed: " + dep);
            return false;
        }
    }
    return true;
}

bool ChainEngine::execute_task(const ChainTask& task) {
    report("Executing task: " + task.id + " [" + task.command + "]");
    for (int attempt = 0; attempt <= task.retries; ++attempt) {
        int result = std::system(task.command.c_str());
        if (result == 0) {
            task_status[task.id] = true;
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    task_status[task.id] = false;
    return false;
}

bool ChainEngine::execute() {
    for (const auto& [id, task] : task_map) {
        if (!resolve_dependencies(task)) continue;
        execute_task(task);
    }
    return true;
}

void ChainEngine::report(const std::string& message) {
    std::cout << "[synq::chain] " << message << std::endl;
}

}