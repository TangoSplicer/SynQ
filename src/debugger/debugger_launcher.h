// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUGGER_LAUNCHER_H
#define SYNQ_DEBUGGER_LAUNCHER_H

#include <string>

namespace synq {

class DebuggerLauncher {
public:
    static void triggerFromError(const std::string& error);
    static void triggerManual();
    static void triggerBreakpoint(const std::string& func);
};

}

#endif // SYNQ_DEBUGGER_LAUNCHER_H