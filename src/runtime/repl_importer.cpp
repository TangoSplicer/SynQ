// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "repl_importer.h"
#include <iostream>

namespace synq {

void REPLImporter::import(const std::string& module) {
    std::cout << "[REPLImporter] Imported module: " << module << "\n";
}

void REPLImporter::unload(const std::string& module) {
    std::cout << "[REPLImporter] Unloaded module: " << module << "\n";
}

}