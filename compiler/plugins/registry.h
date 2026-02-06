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
// File: plugins/registry.h
#pragma once

#include <string>
#include <map>
#include <memory>
#include "compiler/plugin.h"

namespace synq {

class PluginRegistry {
private:
    std::map<std::string, std::shared_ptr<Plugin>> loaded;

public:
    void registerPlugin(const std::shared_ptr<Plugin>& plugin);
    void unloadPlugin(const std::string& name);
    void runPlugin(const std::string& name);
    bool isLoaded(const std::string& name) const;

    template<typename T>
    std::shared_ptr<T> get(const std::string& name) const {
        auto it = loaded.find(name);
        if (it != loaded.end()) {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    static PluginRegistry& instance(); // singleton
};

} // namespace synq