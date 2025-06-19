// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUGSCRIPT_H
#define SYNQ_DEBUGSCRIPT_H

#include <string>
#include <vector>

namespace synq {

class DebugScript {
public:
    static void load(const std::string& script);
    static void run();
    static void clear();

private:
    static std::vector<std::string> scriptLines;
};

}

#endif // SYNQ_DEBUGSCRIPT_H