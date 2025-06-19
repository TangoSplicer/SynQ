// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "session_plugin_loader.h"

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

#include <iostream>

namespace synq {

void SessionPluginLoader::loadPlugin(const std::string& pluginPath) {
#if defined(_WIN32)
    HINSTANCE libHandle = LoadLibrary(pluginPath.c_str());
    if (!libHandle) {
        std::cerr << "[PluginLoader] Failed to load: " << pluginPath << "\n";
        return;
    }
    std::cout << "[PluginLoader] Loaded Windows DLL: " << pluginPath << "\n";
#else
    void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "[PluginLoader] Failed to load: " << pluginPath << " | Error: " << dlerror() << "\n";
        return;
    }
    std::cout << "[PluginLoader] Loaded Unix Shared Library: " << pluginPath << "\n";
#endif
}

}