// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/loader.cpp

#include "loader.h"
#include "registry.h"
#include <iostream>

#if defined(_WIN32)
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace synq {

typedef Plugin* (*CreatePluginFn)();

bool PluginLoader::loadFromFile(const std::string& filepath) {
#if defined(_WIN32)
    HMODULE handle = LoadLibrary(filepath.c_str());
    if (!handle) {
        std::cerr << "[PLUGIN] Failed to load DLL: " << filepath << "\n";
        return false;
    }

    CreatePluginFn create = (CreatePluginFn)GetProcAddress(handle, "create_plugin");
    if (!create) {
        std::cerr << "[PLUGIN] Missing entry point: create_plugin\n";
        return false;
    }
#else
    void* handle = dlopen(filepath.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "[PLUGIN] Failed to load shared library: " << filepath << "\n";
        std::cerr << dlerror() << "\n";
        return false;
    }

    dlerror(); // clear existing errors
    CreatePluginFn create = (CreatePluginFn)dlsym(handle, "create_plugin");
    const char* dlsym_error = dlerror();
    if (dlsym_error) {
        std::cerr << "[PLUGIN] Entry point error: " << dlsym_error << "\n";
        return false;
    }
#endif

    std::shared_ptr<Plugin> plugin(create());
    PluginRegistry::instance().registerPlugin(plugin);

    std::cout << "[PLUGIN] Dynamically loaded: " << plugin->name() << "\n";
    return true;
}

} // namespace synq