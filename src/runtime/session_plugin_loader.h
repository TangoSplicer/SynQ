// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SESSION_PLUGIN_LOADER_H
#define SYNQ_SESSION_PLUGIN_LOADER_H

#include <string>

namespace synq {

class SessionPluginLoader {
public:
    static void loadPlugin(const std::string& pluginPath);
};

}

#endif // SYNQ_SESSION_PLUGIN_LOADER_H