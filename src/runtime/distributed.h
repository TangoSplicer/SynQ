// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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