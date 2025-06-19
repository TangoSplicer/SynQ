// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_HOTRELOAD_WATCHDOG_H
#define SYNQ_HOTRELOAD_WATCHDOG_H

#include <string>
#include <map>

namespace synq {

class HotReloadWatchdog {
public:
    static void watch(const std::string& path);
    static void unwatch(const std::string& path);
    static void checkUpdates();

private:
    static std::map<std::string, long> timestamps;
};

}

#endif // SYNQ_HOTRELOAD_WATCHDOG_H