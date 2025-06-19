// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "session_scriptor.h"
#include <fstream>
#include <iostream>
#include "eval_context.h"

namespace synq {

void SessionScriptor::save(const std::string& path) {
    std::ofstream out(path);
    out << "# SynQ REPL Session Export\n";
    for (const auto& [var, val] : EvalContext::variables) {
        out << var << " = " << val << "\n";
    }
    out.close();
    std::cout << "[SessionScriptor] Saved session to " << path << "\n";
}

void SessionScriptor::load(const std::string& path) {
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        auto eq = line.find('=');
        if (eq != std::string::npos) {
            std::string var = line.substr(0, eq - 1);
            std::string val = line.substr(eq + 2);
            EvalContext::define(var, val);
        }
    }
    in.close();
    std::cout << "[SessionScriptor] Loaded session from " << path << "\n";
}

}