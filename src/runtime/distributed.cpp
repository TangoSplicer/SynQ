// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/runtime/distributed.cpp

#include "distributed.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sstream>
#include <random>

namespace synq {

ComputeNode::ComputeNode(std::string id, NodeType type)
    : id(id), type(type) {}

DistributedResult ComputeNode::execute(const Task& task) {
    std::ostringstream out;
    out << "[Node " << id << "] Executing task: " << task.id;

    // Simulated execution delay
    std::this_thread::sleep_for(std::chrono::milliseconds(50 + (rand() % 100)));

    // Simulated result
    DistributedResult result;
    result.task_id = task.id;
    result.output = "Executed: " + task.code + " on " + id;
    return result;
}

std::string ComputeNode::get_id() const {
    return id;
}

NodeType ComputeNode::get_type() const {
    return type;
}

// -- DistributedScheduler Implementation --

void DistributedScheduler::register_node(ComputeNode node) {
    nodes.push_back(node);
}

ComputeNode DistributedScheduler::select_node(const Task& task) {
    // Naive strategy: match type or fallback
    for (const auto& node : nodes) {
        if (node.get_type() == task.preferred)
            return node;
    }
    return nodes.front(); // fallback
}

std::future<DistributedResult> DistributedScheduler::dispatch_async(const Task& task) {
    ComputeNode node = select_node(task);
    return std::async(std::launch::async, [=]() {
        return node.execute(task);
    });
}

std::vector<DistributedResult> DistributedScheduler::dispatch_batch(const std::vector<Task>& tasks) {
    std::vector<std::future<DistributedResult>> futures;
    std::vector<DistributedResult> results;

    for (const auto& task : tasks) {
        futures.push_back(dispatch_async(task));
    }

    for (auto& f : futures) {
        results.push_back(f.get());
    }

    return results;
}

} // namespace synq