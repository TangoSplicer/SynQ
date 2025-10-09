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

// synq/src/plugins/chain/chain_engine.h
// SynQ Distributed Chain Engine Interface
// License: 

#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace synq::chain {

struct ChainTask {
    std::string id;
    std::string type;
    std::string command;
    std::vector<std::string> depends_on;
    std::map<std::string, std::string> env;
    int retries = 0;
};

class ChainEngine {
public:
    ChainEngine();
    ~ChainEngine();

    bool load_from_file(const std::string& filepath);
    bool execute();

private:
    std::map<std::string, ChainTask> task_map;
    std::map<std::string, bool> task_status;

    bool execute_task(const ChainTask& task);
    bool resolve_dependencies(const ChainTask& task);
    void report(const std::string& message);
};

}