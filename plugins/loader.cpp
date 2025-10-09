// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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