// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// target_registry.cpp
#include "target_registry.h"

namespace synq::compiler {

  std::vector<TargetProfile> TargetRegistry::list_targets() {
    return {
      {"x86_64", "Native Binary", ".exe", TargetType::CLASSICAL},
      {"wasm", "WebAssembly", ".wasm", TargetType::CLASSICAL},
      {"qiskit", "Qiskit QASM", ".qasm", TargetType::QUANTUM},
      {"openqasm3", "OpenQASM 3", ".qasm3", TargetType::QUANTUM},
      {"synqvm", "SynQ Bytecode", ".synqbin", TargetType::HYBRID}
    };
  }

}