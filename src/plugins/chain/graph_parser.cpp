// synq/src/plugins/chain/graph_parser.cpp
// Loads task graph into memory from JSON structure
// License: SynQ Commercial Attribution License v1.0

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