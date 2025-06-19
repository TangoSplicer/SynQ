// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/cli/synq_create.cpp

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <map>

namespace fs = std::filesystem;

const std::string DEFAULT_PLUGIN_TEMPLATE = R"PLUGIN(
plugin.json:
{
  "name": "<plugin_name>",
  "version": "0.1.0",
  "description": "A new SynQ plugin.",
  "entry": "src/<plugin_name>.cpp",
  "sandbox": true,
  "tags": ["custom"]
}

src/<plugin_name>.cpp:
#include "../include/<plugin_name>.h"
#include <synq/plugin.h>

void register_plugin() {
    // Register your gates, passes, or backend hooks here
}

README.md:
# <plugin_name>
Custom SynQ plugin.
)PLUGIN";

// Utility function to write a file with given content
void write_file(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    if (file) {
        file << content;
        file.close();
    }
}

void create_plugin(const std::string& name) {
    std::string base = "./" + name;
    fs::create_directories(base + "/src");
    fs::create_directories(base + "/include");
    fs::create_directories(base + "/tests");

    std::string plugin_json = R"({
  "name": ")" + name + R"(",
  "version": "0.1.0",
  "description": "A new SynQ plugin.",
  "entry": "src/)" + name + R"(.cpp",
  "sandbox": true,
  "tags": ["custom"]
})";

    std::string source_code = "#include \"../include/" + name + ".h\"\n#include <synq/plugin.h>\n\nvoid register_plugin() {\n    // Plugin logic here\n}\n";
    std::string header_code = "#pragma once\n\n// " + name + " plugin interface\n";

    write_file(base + "/plugin.json", plugin_json);
    write_file(base + "/src/" + name + ".cpp", source_code);
    write_file(base + "/include/" + name + ".h", header_code);
    write_file(base + "/README.md", "# " + name + "\nA new SynQ plugin.\n");
    write_file(base + "/tests/test_" + name + ".synq", "// Write tests here\n");

    std::cout << "✅ Plugin '" << name << "' scaffolded successfully.\n";
}

void create_project(const std::string& name) {
    std::string base = "./" + name;
    fs::create_directories(base + "/src");
    fs::create_directories(base + "/examples");
    fs::create_directories(base + "/docs");
    fs::create_directories(base + "/tests");

    write_file(base + "/README.md", "# " + name + "\nYour SynQ project.\n");
    write_file(base + "/src/main.synq", "// Entry point\n");
    write_file(base + "/tests/test_main.synq", "// Basic test\n");

    std::cout << "✅ Project '" << name << "' created.\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: synq create <plugin|project> <name>\n";
        return 1;
    }

    std::string type = argv[1];
    std::string name = argv[2];

    if (type == "plugin") {
        create_plugin(name);
    } else if (type == "project") {
        create_project(name);
    } else {
        std::cerr << "Unknown type '" << type << "'. Use 'plugin' or 'project'.\n";
        return 1;
    }

    return 0;
}