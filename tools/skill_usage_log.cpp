// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
#include <nlohmann/json.hpp>

void logSkillUsage(const std::string& skill, const std::string& plugin, bool success) {
    std::ofstream log("logs/skill_usage.jsonl", std::ios::app);
    if (!log.is_open()) return;

    std::time_t now = std::time(nullptr);
    log << nlohmann::json{
        {"timestamp", now},
        {"skill", skill},
        {"plugin", plugin},
        {"success", success}
    }.dump() << "\n";

    log.close();
}