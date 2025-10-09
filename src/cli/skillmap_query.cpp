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


// Author: SynQ Contributors
// CLI Tool: Skillmap Telemetry Inspector

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const std::string TRACKER_PATH = "build/skillmap_telemetry.json";

void show_all_plugins() {
    std::ifstream file(TRACKER_PATH);
    if (!file) {
        std::cout << "No skillmap data found.\n";
        return;
    }

    json data;
    file >> data;
    for (auto& [plugin, variants] : data.items()) {
        std::cout << "🧩 Plugin: " << plugin << "\n";
        for (auto& [variant, stats] : variants.items()) {
            std::cout << "  ◾ Variant: " << variant << "\n";
            std::cout << "     Uses: " << stats["uses"] << "\n";
            std::cout << "     Accuracy: " << stats["accuracy_avg"] << "\n";
            std::cout << "     Speed: " << stats["speed_avg"] << "\n";
            std::cout << "     Hallucination Rate: " << stats["hallucination_rate"] << "\n";
        }
    }
}

void show_plugin(const std::string& name) {
    std::ifstream file(TRACKER_PATH);
    if (!file) {
        std::cout << "No telemetry available.\n";
        return;
    }
    json data;
    file >> data;
    if (!data.contains(name)) {
        std::cout << "Plugin not found in telemetry: " << name << "\n";
        return;
    }

    auto variants = data[name];
    std::cout << "🔍 Plugin: " << name << "\n";
    for (auto& [variant, stats] : variants.items()) {
        std::cout << "  ◾ Variant: " << variant << "\n";
        std::cout << "     Uses: " << stats["uses"] << "\n";
        std::cout << "     Accuracy: " << stats["accuracy_avg"] << "\n";
        std::cout << "     Speed: " << stats["speed_avg"] << "\n";
        std::cout << "     Hallucination Rate: " << stats["hallucination_rate"] << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        show_all_plugins();
    } else if (argc == 2) {
        show_plugin(argv[1]);
    } else {
        std::cout << "Usage:\n";
        std::cout << "  synq skillmap             → Show all plugin telemetry\n";
        std::cout << "  synq skillmap <plugin>    → Show telemetry for specific plugin\n";
    }
    return 0;
}