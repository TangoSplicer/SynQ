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