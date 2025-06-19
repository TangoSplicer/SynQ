// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_RUNTIME_TIMER_H
#define SYNQ_RUNTIME_TIMER_H

#include <string>
#include <chrono>

namespace synq {

class RuntimeTimer {
public:
    static void start();
    static void stop();
    static std::string duration();

private:
    static std::chrono::high_resolution_clock::time_point start_time;
    static std::chrono::high_resolution_clock::time_point end_time;
};

}

#endif // SYNQ_RUNTIME_TIMER_H