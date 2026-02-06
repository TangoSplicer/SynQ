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
// File: plugins/registry.cpp

#include "registry.h"
#include <iostream>

namespace synq {

void PluginRegistry::registerPlugin(const std::shared_ptr<Plugin>& plugin) {
    std::string id = plugin->name();
    if (loaded.count(id)) {
        std::cout << "[PLUGIN] Already registered: " << id << "\n";
        return;
    }
    plugin->onLoad();
    loaded[id] = plugin;
    std::cout << "[PLUGIN] Loaded: " << id << " v" << plugin->version() << "\n";
}

void PluginRegistry::unloadPlugin(const std::string& name) {
    auto it = loaded.find(name);
    if (it != loaded.end()) {
        it->second->onUnload();
        loaded.erase(it);
        std::cout << "[PLUGIN] Unloaded: " << name << "\n";
    }
}

void PluginRegistry::runPlugin(const std::string& name) {
    if (loaded.count(name)) {
        loaded[name]->run();
    } else {
        std::cerr << "[PLUGIN] Not found: " << name << "\n";
    }
}

bool PluginRegistry::isLoaded(const std::string& name) const {
    return loaded.count(name) > 0;
}

PluginRegistry& PluginRegistry::instance() {
    static PluginRegistry instance;
    return instance;
}

} // namespace synq