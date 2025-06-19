// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: tests/unit/test_mock_plugin.cpp

#include "../../plugins/registry.h"
#include "../../plugins/ai_plugin.h"
#include <iostream>

using namespace synq;

class MockPluginTest : public AIProviderPlugin {
public:
    std::string name() const override { return "TestPlugin"; }
    std::string version() const override { return "0.1"; }

    void onLoad() override { std::cout << "[TestPlugin] onLoad()\n"; }
    void onUnload() override { std::cout << "[TestPlugin] onUnload()\n"; }
    void run() override { std::cout << "[TestPlugin] run()\n"; }

    std::string runModel(const std::string& model, const std::string& prompt) override {
        return "[TestPlugin] prompt received: " + prompt;
    }
};

int main() {
    auto plugin = std::make_shared<MockPluginTest>();
    auto& registry = PluginRegistry::instance();

    registry.registerPlugin(plugin);

    if (!registry.isLoaded("TestPlugin")) {
        std::cerr << "[FAIL] Plugin not registered\n";
        return 1;
    }

    auto test = registry.get<AIProviderPlugin>("TestPlugin");
    if (!test) {
        std::cerr << "[FAIL] Plugin type mismatch\n";
        return 1;
    }

    std::string output = test->runModel("model", "test?");
    std::cout << "Output: " << output << "\n";

    if (output.find("prompt received") == std::string::npos) {
        std::cerr << "[FAIL] Incorrect plugin response\n";
        return 1;
    }

    std::cout << "[PASS] Plugin test succeeded\n";
    return 0;
}