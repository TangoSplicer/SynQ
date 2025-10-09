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
// hotreload.cpp
#include "hotreload.h"
#include "parser.h"
#include "codegen.h"
#include <fstream>
#include <iostream>

namespace synq::compiler {

  HotReloader::HotReloader(Runtime* target) : runtime(target) {}

  void HotReloader::register_module(const std::string& name, const std::string& path) {
    module_registry[name] = path;
  }

  bool HotReloader::reload_module(const std::string& name) {
    if (module_registry.find(name) == module_registry.end()) {
      std::cerr << "⚠️ Module not registered: " << name << std::endl;
      return false;
    }

    std::string path = module_registry[name];
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Failed to open module: " << path << std::endl;
      return false;
    }

    std::string code((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (!trust.verify_fingerprint(code, trust.certs[name].fingerprint)) {
      std::cerr << "❌ Trust failed on hot reload: " << name << std::endl;
      return false;
    }

    Parser parser;
    AST ast = parser.parse(code);
    CodeGenerator codegen;
    Program program = codegen.generate(ast);

    std::cout << "♻️ Reloaded module '" << name << "' successfully.\n";
    runtime->inject(program);
    return true;
  }

}