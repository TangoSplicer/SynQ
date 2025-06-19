// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "callgraphviz.h"
#include <iostream>

namespace synq {

std::map<std::string, CallNode> CallGraphViz::graph;

void CallGraphViz::enter(const std::string& caller, const std::string& callee) {
    graph[caller].name = caller;
    graph[caller].callees.push_back(callee);
}

void CallGraphViz::render() {
    std::cout << "[Call Graph Visualization]\n";
    for (const auto& [caller, node] : graph) {
        std::cout << caller << " -> ";
        for (const auto& callee : node.callees) {
            std::cout << callee << ", ";
        }
        std::cout << "\n";
    }
}

void CallGraphViz::clear() {
    graph.clear();
}

}