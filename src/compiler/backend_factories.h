// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// backend_factories.h
#pragma once
#include "codegen_backend.h"

namespace synq::compiler {
  std::shared_ptr<CodegenBackend> make_backend_x86();
  std::shared_ptr<CodegenBackend> make_backend_wasm();
  std::shared_ptr<CodegenBackend> make_backend_qiskit();
  std::shared_ptr<CodegenBackend> make_backend_synqvm();
}