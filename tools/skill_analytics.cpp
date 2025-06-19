// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include <iostream>
#include <fstream>
#include <map>
#include <nlohmann/json.hpp>

int main() {
    std::ifstream log("logs/skill_usage.jsonl");
    if (!log.is_open()) {
        std::cerr << "No log file found.\n";
        return 1;
    }

    std::map<std::string, int> skill_count;
    std::map<std::string, int> plugin_count;
    std::map<std::string, int> failures;

    std::string line;
    while (std::getline(log, line)) {
        auto json = nlohmann::json::parse(line);
        std::string skill = json["skill"];
        std::string plugin = json["plugin"];
        bool success = json["success"];

        skill_count[skill]++;
        plugin_count[plugin]++;
        if (!success) failures[skill]++;
    }

    std::cout << "🧠 Skill Usage Summary:\n";
    for (const auto& [skill, count] : skill_count) {
        int fail = failures[skill];
        std::cout << "- " << skill << ": " << count << " calls ("
                  << fail << " failed, "
                  << 100 * (1.0 - (double)fail / count) << "% success)\n";
    }

    std::cout << "\n🔌 Plugin Usage Summary:\n";
    for (const auto& [plugin, count] : plugin_count) {
        std::cout << "- " << plugin << ": " << count << " skills used\n";
    }

    return 0;
}