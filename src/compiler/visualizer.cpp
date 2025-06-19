// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/compiler/visualizer.cpp

#include "visualizer.h"
#include <sstream>
#include <unordered_map>
#include <iomanip>

namespace synq {

Visualizer::Visualizer() {}

void Visualizer::load_circuit(const std::string& ir) {
    timeline.clear();
    parse_ir(ir);
}

void Visualizer::parse_ir(const std::string& ir) {
    std::istringstream stream(ir);
    std::string token;
    std::vector<std::vector<std::string>> temp(10, std::vector<std::string>());

    while (stream >> token) {
        if (token == "H") {
            int t;
            stream >> t;
            temp[t].push_back("H");
        } else if (token == "CNOT") {
            int c, t;
            stream >> c >> t;
            temp[c].push_back("C");
            temp[t].push_back("X");
        } else {
            // Pad unknown
        }
    }

    timeline = temp;
}

std::string Visualizer::as_ascii() const {
    std::ostringstream out;
    for (size_t i = 0; i < timeline.size(); ++i) {
        if (timeline[i].empty()) continue;
        out << "q[" << i << "]: ";
        for (const auto& gate : timeline[i]) {
            out << "[" << std::setw(2) << gate << "]";
        }
        out << "\n";
    }
    return out.str();
}

std::string Visualizer::as_dot() const {
    std::ostringstream out;
    out << "digraph Circuit {\n";
    out << "  rankdir=LR;\n";
    out << "  node [shape=box];\n";

    for (size_t q = 0; q < timeline.size(); ++q) {
        if (timeline[q].empty()) continue;
        out << "  subgraph cluster_q" << q << " {\n";
        out << "    label=\"q[" << q << "]\";\n";
        for (size_t t = 0; t < timeline[q].size(); ++t) {
            std::string node = "q" + std::to_string(q) + "t" + std::to_string(t);
            out << "    " << node << " [label=\"" << timeline[q][t] << "\"];\n";
            if (t > 0) {
                std::string prev = "q" + std::to_string(q) + "t" + std::to_string(t - 1);
                out << "    " << prev << " -> " << node << ";\n";
            }
        }
        out << "  }\n";
    }

    out << "}\n";
    return out.str();
}

} // namespace synq