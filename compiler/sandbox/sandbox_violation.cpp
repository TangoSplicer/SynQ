// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// sandbox_violation.cpp
#include "sandbox_violation.h"
#include <chrono>
#include <ctime>

namespace synq::sandbox {

  void ViolationLogger::log(const Violation& v) {
    logbook.push_back(v);
  }

  std::vector<Violation> ViolationLogger::get_all() const {
    return logbook;
  }

  void ViolationLogger::clear() {
    logbook.clear();
  }

}