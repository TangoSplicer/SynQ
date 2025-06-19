// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SESSION_PERSONA_H
#define SYNQ_SESSION_PERSONA_H

#include <string>

namespace synq {

class SessionPersona {
public:
    static void setPersona(const std::string& name);
    static std::string getPersona();
};

}

#endif // SYNQ_SESSION_PERSONA_H