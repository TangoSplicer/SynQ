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