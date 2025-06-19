// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// File: synq_export.cpp — CLI interface for SynQ Exporter

#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc < 4 || std::string(argv[1]) != "export") {
        std::cout << "Usage: synq export <source_file.synq> --target <synqvm|wasm|llvm>\n";
        return 1;
    }

    std::string source = argv[2];
    std::string target = argv[4];

    std::string command = "python3 -c \"import sys; sys.path.append('src/compiler'); "
                          "from exporter import export; export('" + source + "', '" + target + "')\"";

    std::system(command.c_str());
    return 0;
}