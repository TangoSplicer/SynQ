// synq/src/cli/synq_chain.cpp
// CLI Tool: synq chain run <pipeline_file.json>
// License: SynQ Commercial Attribution License v1.0

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