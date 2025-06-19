// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DEBUGGER_AI_ASSIST_H
#define SYNQ_DEBUGGER_AI_ASSIST_H

#include <string>

namespace synq {

class DebuggerAIAssist {
public:
    static void explain(const std::string& errorMsg);
};

}

#endif // SYNQ_DEBUGGER_AI_ASSIST_H