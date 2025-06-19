// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// plugin_loader.cpp
#include "plugin_loader.h"
#include "ast.h"
#include <map>
#include <iostream>
#include <sstream>

static std::map<std::string, PluginInfo> pluginRegistry;
static std::map<std::string, std::function<std::string(std::shared_ptr<ASTNode>)>> exportHooks;

void loadAllPlugins() {
    // Simulated dynamic loading — normally read from file or shared lib
    loadPlugin("qbench.export_runtime_profile");
    loadPlugin("ai.tracer");
    loadPlugin("ast.flow_renderer");
}

bool loadPlugin(const std::string& name) {
    if (pluginRegistry.count(name)) return true;

    PluginInfo info;
    info.name = name;
    info.loaded = true;

    // Simulated metadata
    if (name == "qbench.export_runtime_profile") {
        info.description = "Exports performance stats for quantum kernels";
        info.tags = {"quantum", "benchmark"};
        exportHooks[name] = [](std::shared_ptr<ASTNode> ast) -> std::string {
            return "// Quantum Benchmark Report\n" + ast->profileQKernels();
        };
    } else if (name == "ai.tracer") {
        info.description = "AI-enhanced trace capture with LLM labels";
        info.tags = {"ai", "trace"};
        exportHooks[name] = [](std::shared_ptr<ASTNode> ast) -> std::string {
            return "// AI Trace\n" + ast->annotateWithAI();
        };
    } else if (name == "ast.flow_renderer") {
        info.description = "Renders AST to visual flowgraph format";
        info.tags = {"ast", "visual"};
        exportHooks[name] = [](std::shared_ptr<ASTNode> ast) -> std::string {
            return ast->renderFlowGraph();
        };
    } else {
        info.description = "Generic Plugin";
        info.tags = {"misc"};
        exportHooks[name] = [](std::shared_ptr<ASTNode> ast) {
            return "// Default export for plugin: " + ast->getName();
        };
    }

    pluginRegistry[name] = info;
    std::cout << "✔ Loaded plugin: " << name << "\n";
    return true;
}

bool unloadPlugin(const std::string& name) {
    if (pluginRegistry.count(name)) {
        pluginRegistry.erase(name);
        exportHooks.erase(name);
        std::cout << "✖ Unloaded plugin: " << name << "\n";
        return true;
    }
    return false;
}

std::vector<PluginInfo> listAvailablePlugins() {
    std::vector<PluginInfo> result;
    for (const auto& [_, p] : pluginRegistry)
        result.push_back(p);
    return result;
}

std::string getPluginMetadata(const std::string& pluginName) {
    if (!pluginRegistry.count(pluginName)) return "Not found";
    const auto& p = pluginRegistry[pluginName];
    std::ostringstream oss;
    oss << "Plugin: " << p.name << "\nDesc: " << p.description << "\nTags: ";
    for (const auto& t : p.tags) oss << "#" << t << " ";
    return oss.str();
}

std::string callPluginExport(const std::string& pluginName, std::shared_ptr<ASTNode> ast) {
    if (!exportHooks.count(pluginName)) return "// Plugin export not available\n";
    return exportHooks[pluginName](ast);
}