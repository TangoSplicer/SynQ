// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: backend/ai_provider.h
#pragma once

#include <string>

namespace synq {

// Abstract interface for AI providers
class AIProvider {
public:
    virtual ~AIProvider() = default;

    // Run a model with the given prompt and return raw text output
    virtual std::string run(const std::string& model_name, const std::string& prompt) = 0;
};

} // namespace synq