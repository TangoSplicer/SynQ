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
#include "builtin_loader.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> BuiltinLoader::builtins;

void BuiltinLoader::registerBuiltin(const std::string& name, const std::string& doc) {
    builtins[name] = doc;
}

std::string BuiltinLoader::describe(const std::string& name) {
    return builtins.count(name) ? builtins[name] : "[no doc]";
}

void BuiltinLoader::list() {
    std::cout << "[Built-in Functions]\n";
    for (const auto& [name, doc] : builtins) {
        std::cout << "  " << name << " - " << doc << "\n";
    }
}

}