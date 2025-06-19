// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SESSION_SCRIPTOR_H
#define SYNQ_SESSION_SCRIPTOR_H

#include <string>

namespace synq {

class SessionScriptor {
public:
    static void save(const std::string& path);
    static void load(const std::string& path);
};

}

#endif // SYNQ_SESSION_SCRIPTOR_H