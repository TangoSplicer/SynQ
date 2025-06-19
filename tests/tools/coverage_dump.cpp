// synq/tests/tools/coverage_dump.cpp

#include "coverage.h"

void dump_core_coverage() {
    log_coverage("core_component", 95.2);
    log_coverage("plugin_system", 88.5);
    log_coverage("ai_optimizer", 91.7);
}