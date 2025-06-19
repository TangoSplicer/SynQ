// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "ghostframe.h"
#include <iostream>

namespace synq {

std::vector<GhostFrame> GhostFrame::frameLog;

void GhostFrame::record(int tick, const std::map<std::string, std::string>& state) {
    frameLog.push_back({tick, state});
}

void GhostFrame::rewind(int tick) {
    for (auto it = frameLog.rbegin(); it != frameLog.rend(); ++it) {
        if (it->tickId <= tick) {
            std::cout << "[GhostFrame Rewind to Tick " << it->tickId << "]\n";
            for (const auto& [k, v] : it->state) {
                std::cout << "  " << k << " = " << v << "\n";
            }
            break;
        }
    }
}

void GhostFrame::play() {
    for (const auto& gf : frameLog) {
        std::cout << "[Tick " << gf.tickId << "]\n";
        for (const auto& [k, v] : gf.state) {
            std::cout << "  " << k << " = " << v << "\n";
        }
    }
}

void GhostFrame::reset() {
    frameLog.clear();
}

}