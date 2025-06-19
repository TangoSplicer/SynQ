// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/compiler/visualizer.h

#ifndef SYNQ_COMPILER_VISUALIZER_H
#define SYNQ_COMPILER_VISUALIZER_H

#include <string>
#include <vector>

namespace synq {

class Visualizer {
public:
    Visualizer();

    void load_circuit(const std::string& ir);
    std::string as_ascii() const;
    std::string as_dot() const;

private:
    std::vector<std::vector<std::string>> timeline;
    void parse_ir(const std::string& ir);
};

} // namespace synq

#endif // SYNQ_COMPILER_VISUALIZER_H