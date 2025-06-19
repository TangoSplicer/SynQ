// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_MODEL_PROVIDER_H
#define SYNQ_MODEL_PROVIDER_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <functional>
#include <nlohmann/json.hpp>

namespace synq {

    // Result of a model inference call
    struct InferenceResult {
        std::string output;
        double latency_ms;
        int token_count;
        nlohmann::json raw_response;
    };

    // Abstract interface for model providers
    class ModelProvider {
    public:
        virtual ~ModelProvider() = default;

        // Perform inference with raw prompt and return structured output
        virtual InferenceResult infer(const std::string& prompt,
                                      const nlohmann::json& options = {}) = 0;

        // Stream inference results token-by-token using callback
        virtual void stream_infer(const std::string& prompt,
                                  std::function<void(const std::string&)> on_token,
                                  const nlohmann::json& options = {}) = 0;

        // Return provider name (e.g. "LLaMA3", "OpenAI")
        virtual std::string name() const = 0;
    };

    // Factory for constructing providers
    class ModelProviderRegistry {
    public:
        static ModelProviderRegistry& instance();

        void register_provider(const std::string& key,
                               std::function<std::shared_ptr<ModelProvider>()> factory);

        std::shared_ptr<ModelProvider> create(const std::string& key) const;

        std::vector<std::string> list_available() const;

    private:
        ModelProviderRegistry() = default;
        std::map<std::string, std::function<std::shared_ptr<ModelProvider>()>> registry_;
    };

}  // namespace synq

#endif  // SYNQ_MODEL_PROVIDER_H