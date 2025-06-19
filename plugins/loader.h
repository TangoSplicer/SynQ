// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/loader.h
#pragma once

#include <string>
#include <memory>
#include "plugin.h"

namespace synq {

class PluginLoader {
public:
    // Load plugin from .so/.dll and return pointer (also auto-registers)
    static bool loadFromFile(const std::string& filepath);
};

} // namespace synq