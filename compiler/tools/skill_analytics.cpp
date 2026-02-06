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