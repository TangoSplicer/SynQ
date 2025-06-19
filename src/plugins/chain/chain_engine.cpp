// synq/src/plugins/chain/chain_engine.cpp
// Core Execution Engine for SynQ Chain Pipelines
// License: SynQ Commercial Attribution License v1.0

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