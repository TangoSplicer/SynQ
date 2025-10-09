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
#include "model_provider.h"
#include <stdexcept>
#include <chrono>
#include <thread>  // for mock streaming
#include <iostream>

namespace synq {

    ModelProviderRegistry& ModelProviderRegistry::instance() {
        static ModelProviderRegistry instance;
        return instance;
    }

    void ModelProviderRegistry::register_provider(
        const std::string& key,
        std::function<std::shared_ptr<ModelProvider>()> factory) {
        registry_[key] = factory;
    }

    std::shared_ptr<ModelProvider> ModelProviderRegistry::create(const std::string& key) const {
        auto it = registry_.find(key);
        if (it != registry_.end()) {
            return it->second();
        }
        throw std::runtime_error("Model provider not found: " + key);
    }

    std::vector<std::string> ModelProviderRegistry::list_available() const {
        std::vector<std::string> keys;
        for (const auto& [key, _] : registry_) {
            keys.push_back(key);
        }
        return keys;
    }

    // === MOCK IMPLEMENTATION for DEMO ===

    class MockLlamaProvider : public ModelProvider {
    public:
        InferenceResult infer(const std::string& prompt,
                              const nlohmann::json& options) override {
            return {
                "[Mocked] Answer to: " + prompt,
                12.3,
                static_cast<int>(prompt.size() / 4),
                { {"model", "llama-mock"}, {"prompt", prompt} }
            };
        }

        void stream_infer(const std::string& prompt,
                          std::function<void(const std::string&)> on_token,
                          const nlohmann::json& options) override {
            std::vector<std::string> tokens = {"[", "Mock", "ed]", " Answer", " to:", " ", prompt};
            for (const auto& token : tokens) {
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
                on_token(token);
            }
        }

        std::string name() const override {
            return "MockLlamaProvider";
        }
    };

    // Register mock provider at startup
    struct ProviderInit {
        ProviderInit() {
            ModelProviderRegistry::instance().register_provider("llama-mock", []() {
                return std::make_shared<MockLlamaProvider>();
            });
        }
    } static provider_init;

} // namespace synq