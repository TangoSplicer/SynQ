// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/compiler/plugin.h

#ifndef SYNQ_COMPILER_PLUGIN_H
#define SYNQ_COMPILER_PLUGIN_H

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace synq {

struct PassContext {
    std::string source_code;
    std::string intermediate_repr;
    std::unordered_map<std::string, std::string> metadata;

    void set_ir(const std::string& ir);
    std::string get_ir() const;
    void annotate(const std::string& key, const std::string& value);
    std::string get_annotation(const std::string& key) const;
};

class CompilerPass {
public:
    virtual std::string name() const = 0;
    virtual void run(PassContext& ctx) = 0;
    virtual ~CompilerPass() = default;
};

class CompilerPluginRegistry {
public:
    static CompilerPluginRegistry& instance();
    void register_pass(std::string name, std::function<CompilerPass*()> factory);
    std::vector<std::string> list_passes() const;
    void run_all(PassContext& ctx);

private:
    CompilerPluginRegistry() = default;
    std::unordered_map<std::string, std::function<CompilerPass*()>> pass_factories;
};

} // namespace synq

#endif // SYNQ_COMPILER_PLUGIN_H