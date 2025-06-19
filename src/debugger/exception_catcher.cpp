// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "exception_catcher.h"
#include <iostream>

namespace synq {

std::string ExceptionCatcher::lastMessage = "";
std::string ExceptionCatcher::lastFile = "";
int ExceptionCatcher::lastLine = -1;

void ExceptionCatcher::handle(const std::string& message, const std::string& file, int line) {
    lastMessage = message;
    lastFile = file;
    lastLine = line;
}

void ExceptionCatcher::reportLast() {
    if (lastMessage.empty()) {
        std::cout << "No exceptions caught.\n";
        return;
    }
    std::cout << "[Exception] " << lastMessage << " at " << lastFile << ":" << lastLine << "\n";
}

void ExceptionCatcher::clear() {
    lastMessage.clear();
    lastFile.clear();
    lastLine = -1;
}

}