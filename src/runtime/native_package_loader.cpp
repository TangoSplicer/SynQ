// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "native_package_loader.h"
#include <iostream>

namespace synq {

void NativePackageLoader::install(const std::string& name) {
    std::cout << "[PackageLoader] Installed native package: " << name << "\n";
}

void NativePackageLoader::uninstall(const std::string& name) {
    std::cout << "[PackageLoader] Uninstalled native package: " << name << "\n";
}

}