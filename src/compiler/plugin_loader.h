// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// plugin_loader.h
#pragma once
#include <string>
#include <vector>
#include <memory>

struct PluginInfo {
    std::string name;
    std::string description;
    bool loaded;
    std::vector<std::string> tags;
};

class ASTNode;  // forward declaration

// Core plugin interface
void loadAllPlugins();
bool loadPlugin(const std::string& name);
bool unloadPlugin(const std::string& name);
std::vector<PluginInfo> listAvailablePlugins();
std::string getPluginMetadata(const std::string& pluginName);

// Plugin-driven exports
std::string callPluginExport(const std::string& pluginName, std::shared_ptr<ASTNode> ast);