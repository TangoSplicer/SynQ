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