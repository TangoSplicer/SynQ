// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_PULSEWATCH_H
#define SYNQ_PULSEWATCH_H

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace synq {

struct PulseSnapshot {
    std::chrono::system_clock::time_point timestamp;
    std::map<std::string, std::string> state;
};

class PulseWatch {
public:
    static void capture(const std::map<std::string, std::string>& state);
    static void report();
    static void clear();

private:
    static std::vector<PulseSnapshot> timeline;
};

}

#endif // SYNQ_PULSEWATCH_H