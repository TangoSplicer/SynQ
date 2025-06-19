// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SYMBOL_VISUALIZER_H
#define SYNQ_SYMBOL_VISUALIZER_H

#include <string>

namespace synq {

class SymbolVisualizer {
public:
    static void showAll();
    static void show(const std::string& name);
};

}

#endif // SYNQ_SYMBOL_VISUALIZER_H