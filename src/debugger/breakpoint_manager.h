// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_BREAKPOINT_MANAGER_H
#define SYNQ_BREAKPOINT_MANAGER_H

#include <string>
#include <map>

namespace synq {

class BreakpointManager {
public:
    static void add(const std::string& file, int line);
    static void remove(const std::string& file, int line);
    static bool isActive(const std::string& file, int line);
    static void toggle(const std::string& file, int line);
    static void list();

private:
    static std::map<std::string, std::map<int, bool>> breakpoints;
};

}

#endif // SYNQ_BREAKPOINT_MANAGER_H