// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "context_snapshot.h"
#include "eval_context.h"

#include <iostream>
#include <fstream>
#include <map>

namespace synq {

void ContextSnapshot::save(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "[Snapshot] Failed to save: " << filename << "\n";
        return;
    }
    for (const auto& [k, v] : EvalContext::variables) {
        out << k << "=" << v << "\n";
    }
    out.close();
    std::cout << "[Snapshot] Saved " << EvalContext::variables.size() << " variables to: " << filename << "\n";
}

void ContextSnapshot::restore(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "[Snapshot] Failed to load: " << filename << "\n";
        return;
    }

    std::string line;
    while (std::getline(in, line)) {
        auto eq = line.find('=');
        if (eq != std::string::npos) {
            std::string var = line.substr(0, eq);
            std::string val = line.substr(eq + 1);
            EvalContext::define(var, val);
        }
    }
    in.close();
    std::cout << "[Snapshot] Context restored from " << filename << "\n";
}

}