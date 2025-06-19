// synq/tests/tools/coverage_dump_classical.cpp

#include "coverage.h"

void dump_classical_coverage() {
    log_coverage("std_vector", 98.0);
    log_coverage("control_flow", 96.1);
    log_coverage("type_inference", 93.4);
}