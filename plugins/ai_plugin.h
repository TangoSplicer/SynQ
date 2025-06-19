// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/ai_plugin.h
#pragma once

#include "plugin.h"
#include <string>

namespace synq {

// Interface for AI Model Provider plugins
class AIProviderPlugin : public Plugin {
public:
    virtual std::string runModel(const std::string& modelName, const std::string& prompt) = 0;
};

} // namespace synq