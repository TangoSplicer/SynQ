// SynQ Commercial Attribution License v1.0
#include <iostream>
#include "synq_run.h"
#include "compiler/pipeline_executor.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq_run <file.synq>" << std::endl;
        return 1;
    }

    std::cout << "Running SynQ executable pipeline..." << std::endl;
    synq::compiler::PipelineExecutor executor;
    executor.execute(argv[1]);
    return 0;
}
