// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_EXCEPTION_CATCHER_H
#define SYNQ_EXCEPTION_CATCHER_H

#include <string>

namespace synq {

class ExceptionCatcher {
public:
    static void handle(const std::string& message, const std::string& file, int line);
    static void reportLast();
    static void clear();

private:
    static std::string lastMessage;
    static std::string lastFile;
    static int lastLine;
};

}

#endif // SYNQ_EXCEPTION_CATCHER_H