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
#include "repl.h"
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

namespace synq {

REPL::REPL() {}

REPL::~REPL() {}

std::string REPL::execute(const std::string& command) {
    // Placeholder: interpret DSL command (e.g., @macro or @system call)
    if (command == "@macro list_plugins()") {
        return list_plugins().dump(4);
    }
    return "[!] Unknown DSL command";
}

json REPL::list_plugins() {
    json plugins = json::array();
    for (const auto& [name, meta_pair] : plugin_registry_) {
        json meta = meta_pair.first;
        meta["name"] = name;
        plugins.push_back(meta);
    }
    return plugins;
}

bool REPL::register_plugin(const std::string& name, const std::string& version, const std::string& desc, const std::string& handler) {
    if (plugin_registry_.count(name)) return false;
    json meta;
    meta["version"] = version;
    meta["description"] = desc;
    meta["handler"] = handler;
    plugin_registry_[name] = std::make_pair(meta, nullptr); // handler is a stub for now
    return true;
}

json REPL::invoke_plugin(const std::string& name, const json& payload) {
    if (!plugin_registry_.count(name)) {
        return json{{"error", "plugin not found"}};
    }
    auto handler = plugin_registry_[name].second;
    if (!handler) {
        return json{{"error", "handler not defined"}};
    }
    return handler(payload);
}

} // namespace synq