// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// target_registry.h
#pragma once
#include <vector>
#include "target_profile.h"

namespace synq::compiler {

  class TargetRegistry {
  public:
    static std::vector<TargetProfile> list_targets();
  };

}