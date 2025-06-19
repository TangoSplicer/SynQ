// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "thread_inspector.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> ThreadInspector::threads;

void ThreadInspector::registerThread(const std::string& threadId, const std::string& state) {
    threads[threadId] = state;
}

void ThreadInspector::updateThread(const std::string& threadId, const std::string& state) {
    if (threads.count(threadId)) {
        threads[threadId] = state;
    }
}

void ThreadInspector::listThreads() {
    std::cout << "[Threads]\n";
    for (const auto& [id, state] : threads) {
        std::cout << "  " << id << " - " << state << "\n";
    }
}

void ThreadInspector::clear() {
    threads.clear();
}

}