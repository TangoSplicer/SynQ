// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debug_session_manager.h"
#include <iostream>

namespace synq {

bool DebugSessionManager::sessionActive = false;

void DebugSessionManager::start() {
    sessionActive = true;
    std::cout << "[Debug Session Started]\n";
}

void DebugSessionManager::end() {
    sessionActive = false;
    std::cout << "[Debug Session Ended]\n";
}

bool DebugSessionManager::active() {
    return sessionActive;
}

void DebugSessionManager::info() {
    std::cout << "Debug Active: " << (sessionActive ? "YES" : "NO") << "\n";
}

}