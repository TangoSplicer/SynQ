// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "runtime_heatmap.h"
#include <iostream>
#include <iomanip>

namespace synq {

std::map<std::string, int> RuntimeHeatmap::zones;

void RuntimeHeatmap::mark(const std::string& region, int intensity) {
    zones[region] += intensity;
}

void RuntimeHeatmap::render() {
    std::cout << "[Runtime Heatmap Intensity]\n";
    for (const auto& [region, intensity] : zones) {
        std::cout << "  " << std::setw(15) << std::left << region
                  << ": " << std::string(intensity / 10, '#') << " (" << intensity << ")\n";
    }
}

void RuntimeHeatmap::reset() {
    zones.clear();
}

}