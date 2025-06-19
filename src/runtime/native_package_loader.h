// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_NATIVE_PACKAGE_LOADER_H
#define SYNQ_NATIVE_PACKAGE_LOADER_H

#include <string>

namespace synq {

class NativePackageLoader {
public:
    static void install(const std::string& name);
    static void uninstall(const std::string& name);
};

}

#endif // SYNQ_NATIVE_PACKAGE_LOADER_H