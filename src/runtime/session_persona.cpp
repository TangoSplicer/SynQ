// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "session_persona.h"
#include <iostream>

namespace synq {

static std::string currentPersona = "default";

void SessionPersona::setPersona(const std::string& name) {
    currentPersona = name;
    std::cout << "[Persona] Switched to persona: " << name << "\n";
}

std::string SessionPersona::getPersona() {
    return currentPersona;
}

}