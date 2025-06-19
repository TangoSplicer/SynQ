// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// compiler_job.h
#pragma once
#include <string>
#include <vector>

namespace synq::compiler {

  struct CompilerJob {
    std::string name;
    std::string source_code;
    std::vector<std::string> flags;

    CompilerJob() = default;
    CompilerJob(const std::string& n, const std::string& src, const std::vector<std::string>& f)
      : name(n), source_code(src), flags(f) {}
  };

}