// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_WATCH_LIST_H
#define SYNQ_WATCH_LIST_H

#include <string>
#include <map>

namespace synq {

class WatchList {
public:
    static void watch(const std::string& var);
    static void update(const std::string& var, const std::string& val);
    static void display();
    static void clear();

private:
    static std::map<std::string, std::string> watched;
};

}

#endif // SYNQ_WATCH_LIST_H