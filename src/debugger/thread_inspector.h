// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_THREAD_INSPECTOR_H
#define SYNQ_THREAD_INSPECTOR_H

#include <string>
#include <map>

namespace synq {

class ThreadInspector {
public:
    static void registerThread(const std::string& threadId, const std::string& state);
    static void updateThread(const std::string& threadId, const std::string& state);
    static void listThreads();
    static void clear();

private:
    static std::map<std::string, std::string> threads;
};

}

#endif // SYNQ_THREAD_INSPECTOR_H