// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUGGER_CORE_H
#define SYNQ_DEBUGGER_CORE_H

#include <string>
#include <map>
#include <vector>

namespace synq {

class DebuggerCore {
public:
    static void enter();
    static void defineBreakpoint(const std::string& fn);
    static void removeBreakpoint(const std::string& fn);
    static bool isBreakpoint(const std::string& fn);
    static void runCommand(const std::string& cmd);
    static void printHelp();

private:
    static std::map<std::string, bool> breakpoints;
    static std::vector<std::string> commandHistory;
};

}

#endif // SYNQ_DEBUGGER_CORE_H