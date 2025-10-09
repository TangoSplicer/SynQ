// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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