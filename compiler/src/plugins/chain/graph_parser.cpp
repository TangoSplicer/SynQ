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

// synq/src/plugins/chain/graph_parser.cpp
// Loads task graph into memory from JSON structure
// License: 

#include "graph_parser.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace synq::chain {

std::map<std::string, ChainTask> parse_chain_graph(const std::string& filepath) {
    std::ifstream in(filepath);
    json j;
    in >> j;
    std::map<std::string, ChainTask> tasks;

    for (const auto& node : j["tasks"]) {
        ChainTask t;
        t.id = node["id"];
        t.type = node["type"];
        t.command = node["command"];
        t.retries = node.value("retries", 0);
        for (const auto& d : node.value("depends_on", std::vector<std::string>{}))
            t.depends_on.push_back(d);
        for (auto& [k, v] : node.value("env", json::object()))
            t.env[k] = v;
        tasks[t.id] = t;
    }

    return tasks;
}

}