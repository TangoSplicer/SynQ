// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_LOG_PROBE_H
#define SYNQ_LOG_PROBE_H

#include <string>
#include <vector>

namespace synq {

class LogProbe {
public:
    static void inject(const std::string& msg);
    static void printLog();
    static void clear();

private:
    static std::vector<std::string> log;
};

}

#endif // SYNQ_LOG_PROBE_H