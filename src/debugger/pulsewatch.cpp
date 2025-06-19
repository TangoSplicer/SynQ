// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "pulsewatch.h"
#include <iostream>
#include <iomanip>

namespace synq {

std::vector<PulseSnapshot> PulseWatch::timeline;

void PulseWatch::capture(const std::map<std::string, std::string>& state) {
    timeline.push_back({std::chrono::system_clock::now(), state});
}

void PulseWatch::report() {
    std::cout << "[PulseWatch Timeline]\n";
    for (const auto& snap : timeline) {
        auto time = std::chrono::system_clock::to_time_t(snap.timestamp);
        std::cout << "Snapshot at " << std::put_time(std::localtime(&time), "%F %T") << "\n";
        for (const auto& [k, v] : snap.state) {
            std::cout << "  " << k << " = " << v << "\n";
        }
    }
}

void PulseWatch::clear() {
    timeline.clear();
}

}