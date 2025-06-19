// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// diagnostics.h
#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace synq::lsp {

  nlohmann::json run_diagnostics(const std::string& source);

}