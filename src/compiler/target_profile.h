// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// target_profile.h
#pragma once
#include <string>

namespace synq::compiler {

  enum class TargetType {
    CLASSICAL,
    QUANTUM,
    HYBRID
  };

  struct TargetProfile {
    std::string id;         // "x86_64", "qiskit", "synqvm"
    std::string name;       // Friendly label
    std::string output_ext; // ".exe", ".wasm", ".qasm"
    TargetType type;

    TargetProfile() = default;
    TargetProfile(std::string id, std::string name, std::string ext, TargetType type)
      : id(std::move(id)), name(std::move(name)), output_ext(std::move(ext)), type(type) {}
  };

}