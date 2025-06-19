// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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