// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// runtime.h
#pragma once
#include <vector>
#include <string>

namespace synq::compiler {

  struct Instruction {
    std::string op;
    std::vector<std::string> args;
  };

  struct Program {
    std::vector<Instruction> instructions;
  };

  class Runtime {
  public:
    Runtime();

    void execute(const Program& program);
    void inject(const Program& module);  // 🔁 Hot reload support

    std::vector<Instruction> instructions;
  };

}