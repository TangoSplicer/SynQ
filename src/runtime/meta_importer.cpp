// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "meta_importer.h"
#include <iostream>
#include <map>

namespace synq {

static std::map<std::string, std::string> aliases;

void MetaImporter::importAlias(const std::string& name, const std::string& realModule) {
    aliases[name] = realModule;
    std::cout << "[MetaImport] Alias '" << name << "' now maps to '" << realModule << "'\n";
}

}