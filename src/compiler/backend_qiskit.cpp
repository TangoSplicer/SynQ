// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// backend_qiskit.cpp
#include "codegen_backend.h"
#include <sstream>

namespace synq::compiler {

  class BackendQiskit : public CodegenBackend {
  public:
    std::string generate(const ASTNode& root, const TargetProfile& profile) override {
      std::ostringstream out;
      out << "# SynQ → Qiskit QASM\n";
      out << "OPENQASM 2.0;\n";
      out << "include \"qelib1.inc\";\n";
      out << "qreg q[1];\n";
      out << "h q[0];\n";
      out << "measure q[0] -> c[0];\n";
      return out.str();
    }
  };

  std::shared_ptr<CodegenBackend> make_backend_qiskit() {
    return std::make_shared<BackendQiskit>();
  }

}