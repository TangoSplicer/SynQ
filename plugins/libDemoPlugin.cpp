// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/libDemoPlugin.cpp

#include "ai_plugin.h"
#include <iostream>

using namespace synq;

// A minimal plugin that echoes a transformed prompt
class DemoLLMPlugin : public AIProviderPlugin {
public:
    std::string name() const override { return "DemoLLM"; }
    std::string version() const override { return "1.0"; }

    void onLoad() override {
        std::cout << "[DemoLLM] Plugin loaded successfully.\n";
    }

    void onUnload() override {
        std::cout << "[DemoLLM] Plugin unloaded.\n";
    }

    void run() override {
        std::cout << "[DemoLLM] run() invoked.\n";
    }

    std::string runModel(const std::string& model, const std::string& prompt) override {
        return "[DemoLLM Response] You said: \"" + prompt + "\" (model=" + model + ")";
    }
};

// Entry point required for dynamic loading
extern "C" synq::Plugin* create_plugin() {
    return new DemoLLMPlugin();
}