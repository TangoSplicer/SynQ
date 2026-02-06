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
#include "plugin_manager.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;
using namespace std;

namespace synq {
namespace plugins {

PluginManager::PluginManager() : pluginPath("plugins/installed/") {
    fs::create_directories(pluginPath);
    updateRegistry();
}

void PluginManager::updateRegistry() {
    installed.clear();
    for (auto& f : fs::directory_iterator(pluginPath)) {
        installed[f.path().filename().string()] = f.path().string();
    }
}

void PluginManager::discoverPlugins() {
    cout << "[DISCOVERY] Checking remote and local plugin manifests..." << endl;
    cout << "- Available: ai_expand, qsim_speedup, visual_debugger" << endl; // stubbed
}

void PluginManager::installPlugin(const std::string& name) {
    string path = pluginPath + name;
    ofstream(path) << "# Plugin: " << name << "\n" << "version: 1.0.0" << endl;
    updateRegistry();
    cout << "[✓] Installed plugin: " << name << endl;
}

void PluginManager::loadPlugin(const std::string& name) {
    if (installed.count(name)) {
        cout << "[LOAD] Plugin '" << name << "' activated." << endl;
    } else {
        cout << "[ERROR] Plugin '" << name << "' not found." << endl;
    }
}

void PluginManager::listInstalled() {
    cout << "Installed Plugins:" << endl;
    for (const auto& [k, v] : installed) {
        cout << "- " << k << " @ " << v << endl;
    }
}

void PluginManager::removePlugin(const std::string& name) {
    string path = pluginPath + name;
    if (fs::exists(path)) {
        fs::remove(path);
        updateRegistry();
        cout << "[✓] Removed plugin: " << name << endl;
    } else {
        cout << "[ERROR] Plugin not found: " << name << endl;
    }
}

} // namespace plugins
} // namespace synq