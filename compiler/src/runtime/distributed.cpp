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