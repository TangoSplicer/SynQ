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
#ifndef SYNQ_PLUGIN_STORE_H
#define SYNQ_PLUGIN_STORE_H

#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include <optional>

namespace synq {

    using PluginFunction = std::function<nlohmann::json(const nlohmann::json&)>;

    struct PluginMetadata {
        std::string name;
        std::string version;
        std::string description;
    };

    class CompilerPlugin {
    public:
        CompilerPlugin(PluginMetadata metadata, PluginFunction handler);

        nlohmann::json invoke(const nlohmann::json& input) const;
        PluginMetadata get_metadata() const;

    private:
        PluginMetadata metadata_;
        PluginFunction handler_;
    };

    class PluginStore {
    public:
        static PluginStore& instance();

        void register_plugin(const std::string& name, CompilerPlugin plugin);
        std::vector<std::string> list_plugins() const;
        std::optional<CompilerPlugin> get_plugin(const std::string& name) const;

    private:
        std::map<std::string, CompilerPlugin> plugins_;
    };

} // namespace synq

#endif  // SYNQ_PLUGIN_STORE_H