// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SYNTRACE_H
#define SYNQ_SYNTRACE_H

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace synq {

struct TraceFrame {
    std::string functionName;
    std::string file;
    int line;
    std::map<std::string, std::string> locals;
    std::string quantumStateSnapshot;
};

class SynTrace {
public:
    static void enter(const std::string& function, const std::string& file, int line);
    static void exit();
    static void snapshot(const std::string& var, const std::string& val);
    static void quantumState(const std::string& snapshot);
    static void printTrace();
    static void clear();

private:
    static std::vector<TraceFrame> callStack;
};

} // namespace synq

#endif // SYNQ_SYNTRACE_H