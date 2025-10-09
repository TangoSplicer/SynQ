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
#ifndef SYNQ_PLUGIN_REGISTRY_H
#define SYNQ_PLUGIN_REGISTRY_H

#include <string>
#include <vector>
#include <map>

struct PluginInfo {
    std::string name;
    std::string description;
    std::string version;
    std::vector<std::string> tags;
    int usage_count = 0;
    bool is_deprecated = false;
    bool is_experimental = false;
};

class PluginRegistry {
public:
    void registerPlugin(const PluginInfo& plugin);
    PluginInfo* getPlugin(const std::string& name);
    std::vector<PluginInfo> search(const std::string& query);
    void incrementUsage(const std::string& name);
    std::vector<PluginInfo> getAll();

private:
    std::map<std::string, PluginInfo> registry;
};

#endif