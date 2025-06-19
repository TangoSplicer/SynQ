// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// exporter.h
#pragma once
#include <string>
#include <vector>
#include <memory>

enum class ExportFormat {
    AST,
    QASM,
    QIR,
    IR,
    JSON,
    BINARY,
    TRACE,
    PROFILE,
    FLOWGRAPH,
    TREE,
    AI_TRACE,
    BENCHMARK
};

struct ExportOptions {
    ExportFormat format;
    std::string outputPath;
    bool overwrite = false;
    bool includeMetadata = true;
    std::string plugin = ""; // optional plugin name
};

class Exporter {
public:
    static bool exportProgram(const std::string& sourceCode, const ExportOptions& options);
    static std::vector<std::string> supportedFormats();
};