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