// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "hotreload_watchdog.h"
#include <filesystem>
#include <iostream>

namespace synq {

std::map<std::string, long> HotReloadWatchdog::timestamps;

void HotReloadWatchdog::watch(const std::string& path) {
    long time = std::filesystem::last_write_time(path).time_since_epoch().count();
    timestamps[path] = time;
    std::cout << "[HotReload] Watching: " << path << "\n";
}

void HotReloadWatchdog::unwatch(const std::string& path) {
    timestamps.erase(path);
    std::cout << "[HotReload] Stopped watching: " << path << "\n";
}

void HotReloadWatchdog::checkUpdates() {
    for (auto& [path, time] : timestamps) {
        long new_time = std::filesystem::last_write_time(path).time_since_epoch().count();
        if (new_time > time) {
            std::cout << "[HotReload] Detected change in: " << path << "\n";
            time = new_time;
        }
    }
}

}