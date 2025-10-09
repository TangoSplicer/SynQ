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
#include "plugin_store.h"
#include <stdexcept>

namespace synq {

    CompilerPlugin::CompilerPlugin(PluginMetadata metadata, PluginFunction handler)
        : metadata_(std::move(metadata)), handler_(std::move(handler)) {}

    nlohmann::json CompilerPlugin::invoke(const nlohmann::json& input) const {
        return handler_(input);
    }

    PluginMetadata CompilerPlugin::get_metadata() const {
        return metadata_;
    }

    PluginStore& PluginStore::instance() {
        static PluginStore instance;
        return instance;
    }

    void PluginStore::register_plugin(const std::string& name, CompilerPlugin plugin) {
        plugins_[name] = std::move(plugin);
    }

    std::vector<std::string> PluginStore::list_plugins() const {
        std::vector<std::string> keys;
        for (const auto& [key, _] : plugins_) {
            keys.push_back(key);
        }
        return keys;
    }

    std::optional<CompilerPlugin> PluginStore::get_plugin(const std::string& name) const {
        auto it = plugins_.find(name);
        if (it != plugins_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

} // namespace synq