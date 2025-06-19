// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// exporter.cpp
#include "exporter.h"
#include "ast.h"
#include "parser.h"
#include "runtime.h"
#include "plugin_loader.h"
#include <fstream>
#include <iostream>

static void writeOrWarn(std::ofstream& outFile, const std::string& path, bool overwrite) {
    std::ifstream test(path);
    if (test.good() && !overwrite) {
        std::cerr << "⚠ File already exists: " << path << "\n";
        throw std::runtime_error("overwrite disabled");
    }
    outFile.open(path);
    if (!outFile.is_open()) throw std::runtime_error("failed to open output");
}

bool Exporter::exportProgram(const std::string& sourceCode, const ExportOptions& options) {
    auto tree = parse(sourceCode);
    std::ofstream outFile;

    try {
        writeOrWarn(outFile, options.outputPath, options.overwrite);
    } catch (...) {
        return false;
    }

    switch (options.format) {
        case ExportFormat::AST:
            outFile << tree->toString(); break;
        case ExportFormat::IR:
            outFile << tree->toIR(); break;
        case ExportFormat::QASM:
            outFile << tree->toQASM(); break;
        case ExportFormat::QIR:
            outFile << tree->toQIR(); break;
        case ExportFormat::JSON:
            outFile << tree->toJSON(); break;
        case ExportFormat::BINARY:
            outFile << tree->toBinaryBlob(); break;

        // Plugin-driven exports
        case ExportFormat::TRACE:
        case ExportFormat::PROFILE:
        case ExportFormat::AI_TRACE:
        case ExportFormat::BENCHMARK:
        case ExportFormat::TREE:
        case ExportFormat::FLOWGRAPH:
            if (options.plugin.empty()) {
                std::cerr << "Plugin exporter required for format.\n";
                return false;
            }
            {
                std::string data = callPluginExport(options.plugin, tree);
                outFile << data;
            }
            break;
    }

    if (options.includeMetadata)
        outFile << "\n// Exported via SynQ Exporter\n";

    outFile.close();
    std::cout << "✅ Export successful: " << options.outputPath << "\n";
    return true;
}

std::vector<std::string> Exporter::supportedFormats() {
    return {
        "AST", "IR", "QASM", "QIR", "JSON", "BINARY",
        "TRACE", "PROFILE", "FLOWGRAPH", "TREE",
        "AI_TRACE", "BENCHMARK"
    };
}