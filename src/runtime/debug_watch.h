// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUG_WATCH_H
#define SYNQ_DEBUG_WATCH_H

#include <string>

namespace synq {

class DebugWatch {
public:
    static void watch(const std::string& variable);
    static void unwatch(const std::string& variable);
    static void list();
};

}

#endif // SYNQ_DEBUG_WATCH_H