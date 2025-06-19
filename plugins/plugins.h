// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: plugins/plugin.h
#pragma once

#include <string>

namespace synq {

// Abstract base for all plugin types
class Plugin {
public:
    virtual ~Plugin() = default;

    // Plugin name and version
    virtual std::string name() const = 0;
    virtual std::string version() const = 0;

    // Lifecycle hooks
    virtual void onLoad() = 0;
    virtual void onUnload() = 0;

    // Entry point
    virtual void run() = 0;
};

} // namespace synq