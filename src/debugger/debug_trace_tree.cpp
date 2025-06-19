// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debug_trace_tree.h"
#include <iostream>

namespace synq {

std::shared_ptr<TraceNode> DebugTraceTree::root = std::make_shared<TraceNode>("main", nullptr);
TraceNode* DebugTraceTree::current = root.get();

void DebugTraceTree::enterFunction(const std::string& fn) {
    auto node = std::make_shared<TraceNode>(fn, current);
    current->children.push_back(node);
    current = node.get();
}

void DebugTraceTree::exitFunction() {
    if (current->parent)
        current = current->parent;
}

void printNode(const TraceNode* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "↳ " << node->function << "\n";
    for (const auto& child : node->children) {
        printNode(child.get(), depth + 1);
    }
}

void DebugTraceTree::printTrace() {
    std::cout << "[TraceTree]\n";
    printNode(root.get(), 0);
}

void DebugTraceTree::reset() {
    root = std::make_shared<TraceNode>("main", nullptr);
    current = root.get();
}

}