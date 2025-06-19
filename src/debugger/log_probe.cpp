// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "log_probe.h"
#include <iostream>

namespace synq {

std::vector<std::string> LogProbe::log;

void LogProbe::inject(const std::string& msg) {
    log.push_back(msg);
}

void LogProbe::printLog() {
    std::cout << "[Log Probes]\n";
    for (const auto& l : log) {
        std::cout << "  " << l << "\n";
    }
}

void LogProbe::clear() {
    log.clear();
}

}