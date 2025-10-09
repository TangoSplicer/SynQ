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