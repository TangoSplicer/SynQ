// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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