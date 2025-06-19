// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/compiler/plugin.cpp

#include "plugin.h"
#include <iostream>

namespace synq {

// --- PassContext ---

void PassContext::set_ir(const std::string& ir) {
    intermediate_repr = ir;
}

std::string PassContext::get_ir() const {
    return intermediate_repr;
}

void PassContext::annotate(const std::string& key, const std::string& value) {
    metadata[key] = value;
}

std::string PassContext::get_annotation(const std::string& key) const {
    auto it = metadata.find(key);
    return (it != metadata.end()) ? it->second : "";
}

// --- CompilerPluginRegistry ---

CompilerPluginRegistry& CompilerPluginRegistry::instance() {
    static CompilerPluginRegistry registry;
    return registry;
}

void CompilerPluginRegistry::register_pass(std::string name, std::function<CompilerPass*()> factory) {
    pass_factories[name] = factory;
}

std::vector<std::string> CompilerPluginRegistry::list_passes() const {
    std::vector<std::string> names;
    for (const auto& [name, _] : pass_factories) {
        names.push_back(name);
    }
    return names;
}

void CompilerPluginRegistry::run_all(PassContext& ctx) {
    for (const auto& [name, factory] : pass_factories) {
        CompilerPass* pass = factory();
        std::cout << "[PLUGIN] Running pass: " << pass->name() << "\n";
        pass->run(ctx);
        delete pass;
    }
}

} // namespace synq