// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "builtin_loader.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> BuiltinLoader::builtins;

void BuiltinLoader::registerBuiltin(const std::string& name, const std::string& doc) {
    builtins[name] = doc;
}

std::string BuiltinLoader::describe(const std::string& name) {
    return builtins.count(name) ? builtins[name] : "[no doc]";
}

void BuiltinLoader::list() {
    std::cout << "[Built-in Functions]\n";
    for (const auto& [name, doc] : builtins) {
        std::cout << "  " << name << " - " << doc << "\n";
    }
}

}