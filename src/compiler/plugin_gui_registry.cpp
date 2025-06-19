// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// plugin_gui_registry.cpp
#include "plugin_loader.h"
#include "plugin_registry.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <map>

void renderPluginRegistryUI() {
    auto plugins = listAvailablePlugins();
    std::map<std::string, std::vector<std::string>> tagMap;

    std::cout << "🧩 SynQ Plugin Registry\n";
    std::cout << "--------------------------\n";

    if (plugins.empty()) {
        std::cout << "No plugins installed.\n";
        return;
    }

    for (const auto& plugin : plugins) {
        std::cout << "🟢 " << plugin.name;
        if (plugin.loaded)
            std::cout << " [✔ Loaded]";
        else
            std::cout << " [✖ Not Loaded]";
        std::cout << "\n   └─ " << plugin.description << "\n";

        for (const auto& tag : plugin.tags)
            tagMap[tag].push_back(plugin.name);
    }

    std::cout << "\n📌 Tag Filter:\n";
    for (const auto& [tag, list] : tagMap) {
        std::cout << "- #" << tag << " (" << list.size() << ")\n";
    }

    std::cout << "\n💡 Actions:\n";
    std::cout << " - Type `plugin install <name>` to install.\n";
    std::cout << " - Type `plugin remove <name>` to remove.\n";
    std::cout << " - Type `plugin info <name>` for metadata.\n";
}