// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/classical.h
#pragma once

#include <map>
#include <string>
#include <functional>

namespace synq {
namespace classical {

using ParamMap = std::map<std::string, double>;
using Objective = std::function<double(const ParamMap&)>;

// Standard optimizers
ParamMap gridSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap randomSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap nelderMeadSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap evolutionSearch(const Objective& f, const ParamMap& init, int max_iters);
ParamMap bracketedSearch(const Objective& f, const ParamMap& init, int max_iters);

// Unique optimizer
ParamMap echoWaveSearch(const Objective& f, const ParamMap& init, int max_iters);

} // namespace classical
} // namespace synq