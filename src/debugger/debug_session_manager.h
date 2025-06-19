// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUG_SESSION_MANAGER_H
#define SYNQ_DEBUG_SESSION_MANAGER_H

#include <string>

namespace synq {

class DebugSessionManager {
public:
    static void start();
    static void end();
    static bool active();
    static void info();

private:
    static bool sessionActive;
};

}

#endif // SYNQ_DEBUG_SESSION_MANAGER_H