// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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