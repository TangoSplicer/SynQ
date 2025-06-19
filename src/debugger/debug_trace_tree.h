// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUG_TRACE_TREE_H
#define SYNQ_DEBUG_TRACE_TREE_H

#include <string>
#include <vector>
#include <memory>

namespace synq {

struct TraceNode {
    std::string function;
    std::vector<std::shared_ptr<TraceNode>> children;
    TraceNode* parent;

    TraceNode(const std::string& fn, TraceNode* p = nullptr)
        : function(fn), parent(p) {}
};

class DebugTraceTree {
public:
    static void enterFunction(const std::string& fn);
    static void exitFunction();
    static void printTrace();
    static void reset();

private:
    static std::shared_ptr<TraceNode> root;
    static TraceNode* current;
};

}

#endif // SYNQ_DEBUG_TRACE_TREE_H