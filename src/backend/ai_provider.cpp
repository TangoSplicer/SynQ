// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: backend/ai_provider.cpp

#include "ai_provider.h"
#include <iostream>
#include <memory>

namespace synq {

// Example local mock provider implementation
class MockProvider : public AIProvider {
public:
    std::string run(const std::string& model_name, const std::string& prompt) override {
        std::cout << "[AIProvider] Model: " << model_name << ", Prompt: " << prompt << "\n";
        return "Mocked response for: " + prompt;
    }
};

// Optional factory pattern
std::unique_ptr<AIProvider> createMockProvider() {
    return std::make_unique<MockProvider>();
}

} // namespace synq