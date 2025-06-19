// synq/tests/plugins/chain/chain_test.cpp
// Test Suite for the SynQ Chain Distributed Pipeline Executor
// License: SynQ Commercial Attribution License v1.0

#include "../../../src/plugins/chain/chain_engine.h"
#include <iostream>
#include <fstream>
#include <cassert>

using namespace synq::chain;

void write_sample_pipeline(const std::string& filename) {
    std::ofstream out(filename);
    out << R"JSON(
    {
      "tasks": [
        {
          "id": "prepare",
          "type": "shell",
          "command": "echo Preparing environment",
          "retries": 1
        },
        {
          "id": "run_step",
          "type": "shell",
          "command": "echo Running step logic",
          "depends_on": ["prepare"],
          "retries": 0
        },
        {
          "id": "finalize",
          "type": "shell",
          "command": "echo Finalizing...",
          "depends_on": ["run_step"]
        }
      ]
    }
    )JSON";
    out.close();
}

void test_chain_execution() {
    std::string test_pipeline = "test_chain_pipeline.json";
    write_sample_pipeline(test_pipeline);

    ChainEngine engine;
    assert(engine.load_from_file(test_pipeline) && "[!] Failed to load chain pipeline");
    bool result = engine.execute();
    assert(result && "[!] Execution failed");

    std::cout << "[✓] test_chain_execution passed.\n";
}

int main() {
    std::cout << "[*] Running SynQ Chain tests...\n";
    test_chain_execution();
    std::cout << "[✓] All chain tests passed.\n";
    return 0;
}