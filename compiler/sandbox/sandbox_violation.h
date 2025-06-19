// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// sandbox_violation.h
#pragma once
#include <string>
#include <vector>

namespace synq::sandbox {

  struct Violation {
    std::string plugin_id;
    std::string message;
    std::string node_type;
    std::string timestamp;
  };

  class ViolationLogger {
  public:
    void log(const Violation& v);
    std::vector<Violation> get_all() const;
    void clear();
  private:
    std::vector<Violation> logbook;
  };
}