// SynQ Commercial Attribution License v1.0
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