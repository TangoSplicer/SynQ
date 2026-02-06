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