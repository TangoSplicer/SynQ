// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "watch_list.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> WatchList::watched;

void WatchList::watch(const std::string& var) {
    watched[var] = "";
}

void WatchList::update(const std::string& var, const std::string& val) {
    if (watched.count(var)) {
        watched[var] = val;
    }
}

void WatchList::display() {
    std::cout << "[Watched Variables]\n";
    for (const auto& [var, val] : watched) {
        std::cout << "  " << var << " = " << val << "\n";
    }
}

void WatchList::clear() {
    watched.clear();
}

}