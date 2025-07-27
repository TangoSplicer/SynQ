// SynQ Commercial Attribution License v1.0
#include <iostream>
#include "synq_trace.h"
#include "debugger/qtrace.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: synq_trace <file.synq>" << std::endl;
        return 1;
    }

    std::cout << "Tracing quantum circuit and causal structure..." << std::endl;
    synq::debugger::QTrace::visualize(argv[1]);
    return 0;
}
