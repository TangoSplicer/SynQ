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

// synq/tests/plugins/chain/chain_test.cpp
// Test Suite for the SynQ Chain Distributed Pipeline Executor
// License: 

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