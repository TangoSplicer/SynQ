// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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