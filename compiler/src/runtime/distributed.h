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
// File: src/runtime/distributed.h

#ifndef SYNQ_RUNTIME_DISTRIBUTED_H
#define SYNQ_RUNTIME_DISTRIBUTED_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>
#include <future>

namespace synq {

struct DistributedResult {
    std::string task_id;
    std::string output;
};

enum class NodeType {
    CPU,
    GPU,
    QUANTUM_SIM,
    QPU
};

struct Task {
    std::string id;
    std::string code;
    NodeType preferred;
};

class ComputeNode {
public:
    ComputeNode(std::string id, NodeType type);
    DistributedResult execute(const Task& task);

    std::string get_id() const;
    NodeType get_type() const;

private:
    std::string id;
    NodeType type;
};

class DistributedScheduler {
public:
    void register_node(ComputeNode node);
    std::future<DistributedResult> dispatch_async(const Task& task);
    std::vector<DistributedResult> dispatch_batch(const std::vector<Task>& tasks);

private:
    std::vector<ComputeNode> nodes;
    ComputeNode select_node(const Task& task);
};

} // namespace synq

#endif // SYNQ_RUNTIME_DISTRIBUTED_H