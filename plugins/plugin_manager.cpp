// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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