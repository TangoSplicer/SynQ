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
#include "plugin_registry.h"
#include <algorithm>

void PluginRegistry::registerPlugin(const PluginInfo& plugin) {
    registry[plugin.name] = plugin;
}

PluginInfo* PluginRegistry::getPlugin(const std::string& name) {
    auto it = registry.find(name);
    if (it != registry.end()) return &it->second;
    return nullptr;
}

std::vector<PluginInfo> PluginRegistry::search(const std::string& query) {
    std::vector<PluginInfo> results;
    for (const auto& [name, info] : registry) {
        if (name.find(query) != std::string::npos ||
            info.description.find(query) != std::string::npos ||
            std::any_of(info.tags.begin(), info.tags.end(), [&](const std::string& tag) {
                return tag.find(query) != std::string::npos;
            })) {
            results.push_back(info);
        }
    }
    return results;
}

void PluginRegistry::incrementUsage(const std::string& name) {
    if (registry.count(name)) {
        registry[name].usage_count++;
    }
}

std::vector<PluginInfo> PluginRegistry::getAll() {
    std::vector<PluginInfo> all;
    for (const auto& [_, info] : registry) {
        all.push_back(info);
    }
    return all;
}