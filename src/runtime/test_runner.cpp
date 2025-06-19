// synq/src/runtime/test_runner.cpp

#include "repl_test_runner.h"
#include "coverage.h"
#include <iostream>

int main() {
    std::cout << "Starting SynQ test runner..." << std::endl;
    run_all_unit_tests();
    run_all_integration_tests();
    dump_coverage_report();
    std::cout << "All tests completed." << std::endl;
    return 0;
}