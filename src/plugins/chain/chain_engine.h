// synq/src/plugins/chain/chain_engine.h
// SynQ Distributed Chain Engine Interface
// License: SynQ Commercial Attribution License v1.0

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