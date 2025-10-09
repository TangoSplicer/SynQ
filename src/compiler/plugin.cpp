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