// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_CALLGRAPHVIZ_H
#define SYNQ_CALLGRAPHVIZ_H

#include <string>
#include <vector>
#include <map>

namespace synq {

struct CallNode {
    std::string name;
    std::vector<std::string> callees;
};

class CallGraphViz {
public:
    static void enter(const std::string& caller, const std::string& callee);
    static void render();
    static void clear();

private:
    static std::map<std::string, CallNode> graph;
};

}

#endif // SYNQ_CALLGRAPHVIZ_H