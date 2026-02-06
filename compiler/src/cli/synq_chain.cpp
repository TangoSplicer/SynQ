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

// synq/src/cli/synq_chain.cpp
// CLI Tool: synq chain run <pipeline_file.json>
// License: 

#include <iostream>
#include <string>
#include "../plugins/chain/chain_engine.h"

using namespace synq::chain;

void print_usage() {
    std::cout << "\nSynQ Distributed Pipeline Executor\n";
    std::cout << "Usage:\n";
    std::cout << "  synq chain run <pipeline_file.json>\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return 1;
    }

    std::string subcommand = argv[1];
    std::string filename   = argv[2];

    if (subcommand != "run") {
        print_usage();
        return 1;
    }

    ChainEngine engine;
    if (!engine.load_from_file(filename)) {
        std::cerr << "[✗] Failed to load pipeline file: " << filename << "\n";
        return 1;
    }

    std::cout << "[*] Executing SynQ Pipeline: " << filename << "\n";
    bool success = engine.execute();

    if (success)
        std::cout << "[✓] Pipeline executed successfully.\n";
    else
        std::cerr << "[✗] Pipeline failed. See logs above.\n";

    return success ? 0 : 1;
}