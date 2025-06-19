// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// copilot_whisper.cpp
#include "copilot.h"

std::string whisper_copilot(const std::vector<std::string>& history) {
    Copilot copilot;
    return copilot.whisper(history);
}