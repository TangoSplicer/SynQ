// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "runtime_timer.h"
#include <sstream>

namespace synq {

std::chrono::high_resolution_clock::time_point RuntimeTimer::start_time;
std::chrono::high_resolution_clock::time_point RuntimeTimer::end_time;

void RuntimeTimer::start() {
    start_time = std::chrono::high_resolution_clock::now();
}

void RuntimeTimer::stop() {
    end_time = std::chrono::high_resolution_clock::now();
}

std::string RuntimeTimer::duration() {
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::stringstream ss;
    ss << elapsed.count() << " ms";
    return ss.str();
}

}