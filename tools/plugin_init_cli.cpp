// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include <iostream>
#include <fstream>
#include <string>

void writePlugin(const std::string& name, const std::string& desc) {
    std::string filename = name + ".synq";
    std::ofstream file(filename);
    file << "// Plugin: " << filename << "\n";
    file << "// Description: " << desc << "\n\n";
    file << "plugin " << name << " {\n";
    file << "  version: \"1.0.0\"\n";
    file << "  description: \"" << desc << "\"\n\n";
    file << "  macro run(input) {\n";
    file << "    // your logic here\n";
    file << "    return input;\n";
    file << "  }\n";
    file << "}\n";
    file.close();
    std::cout << "[✓] Plugin created: " << filename << std::endl;
}

int main() {
    std::string name, desc;
    std::cout << "Plugin Name: ";
    std::getline(std::cin, name);
    std::cout << "Plugin Description: ";
    std::getline(std::cin, desc);
    writePlugin(name, desc);
    return 0;
}