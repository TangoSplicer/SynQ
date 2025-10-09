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
// plugin_loader.h
#pragma once
#include <string>
#include <vector>
#include <memory>

struct PluginMetadata {
    std::string name;
    std::string description;
    bool loaded;
    std::vector<std::string> tags;
};

class ASTNode;  // forward declaration

// Core plugin interface
void loadAllPlugins();
bool loadPlugin(const std::string& name);
bool unloadPlugin(const std::string& name);
std::vector<PluginMetadata> listAvailablePlugins();
std::string getPluginMetadata(const std::string& pluginName);

// Plugin-driven exports
std::string callPluginExport(const std::string& pluginName, std::shared_ptr<ASTNode> ast);