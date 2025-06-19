// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "runtime_exporter.h"
#include <iostream>
#include <fstream>

namespace synq {

void RuntimeExporter::exportCode(const std::string& filename) {
    std::ofstream out(filename);
    out << "// Exported SynQ session code\n";
    out << "def main() { print(\"Hello from SynQ Export\") }\n";
    out.close();
    std::cout << "[Exporter] Code written to " << filename << "\n";
}

void RuntimeExporter::exportGraph(const std::string& filename) {
    std::ofstream out(filename);
    out << "<svg><circle r='5'/></svg>\n";
    out.close();
    std::cout << "[Exporter] Graph exported to " << filename << "\n";
}

}