// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/compiler/transpiler.h

#ifndef SYNQ_COMPILER_TRANSPILER_H
#define SYNQ_COMPILER_TRANSPILER_H

#include <string>
#include <vector>
#include <unordered_map>

namespace synq {

enum class TargetFormat {
    QASM,
    CIRQ,
    BRAKET
};

class Transpiler {
public:
    Transpiler();

    void load_ir(const std::string& ir);
    std::string export_to(TargetFormat format);

private:
    std::string ir;
    std::vector<std::string> tokens;

    void tokenize();
    std::string to_qasm();
    std::string to_cirq();
    std::string to_braket();
};

} // namespace synq

#endif // SYNQ_COMPILER_TRANSPILER_H