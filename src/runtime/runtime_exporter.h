// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_RUNTIME_EXPORTER_H
#define SYNQ_RUNTIME_EXPORTER_H

#include <string>

namespace synq {

class RuntimeExporter {
public:
    static void exportCode(const std::string& filename);
    static void exportGraph(const std::string& filename);
};

}

#endif // SYNQ_RUNTIME_EXPORTER_H