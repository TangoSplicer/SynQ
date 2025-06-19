// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// hover_info.h
#pragma once
#include <string>

namespace synq::lsp {

  std::string get_hover_info(const std::string& code, int line, int char_pos);

}