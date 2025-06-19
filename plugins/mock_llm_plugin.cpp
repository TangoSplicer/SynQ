// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/mock_llm_plugin.cpp

#include "ai_plugin.h"
#include "registry.h"
#include <iostream>

namespace synq {

class MockLLMPlugin : public AIProviderPlugin {
public:
    std::string name() const override { return "MockLLM"; }
    std::string version() const override { return "1.0"; }

    void onLoad() override {
        std::cout << "[MockLLM] Loaded.\n";
    }

    void onUnload() override {
        std::cout << "[MockLLM] Unloaded.\n";
    }

    void run() override {
        std::cout << "[MockLLM] Running plugin entry point.\n";
    }

    std::string runModel(const std::string& modelName, const std::string& prompt) override {
        return "[MockLLM] Echo: " + prompt;
    }
};

// Auto-registration
static bool registered = [] {
    PluginRegistry::instance().registerPlugin(std::make_shared<MockLLMPlugin>());
    return true;
}();

} // namespace synq