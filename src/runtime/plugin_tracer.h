// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_PLUGIN_TRACER_H
#define SYNQ_PLUGIN_TRACER_H

#include <string>

namespace synq {

class PluginTracer {
public:
    static void track(const std::string& pluginName);
    static void printTrace();
};

}

#endif // SYNQ_PLUGIN_TRACER_H