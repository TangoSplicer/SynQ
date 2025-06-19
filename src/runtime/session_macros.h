// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SESSION_MACROS_H
#define SYNQ_SESSION_MACROS_H

#include <string>

namespace synq {

class SessionMacros {
public:
    static bool handle(const std::string& line);
};

}

#endif // SYNQ_SESSION_MACROS_H