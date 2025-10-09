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
    // Write source code to temporary file for parsing
       std::string tempFile = "/tmp/synq_temp_export.synq";
       std::ofstream temp(tempFile);
       if (!temp.is_open()) return false;
       temp << sourceCode;
       temp.close();
       
       Parser parser;
       ASTNode* tree = parser.parseFile(tempFile);
       std::remove(tempFile.c_str());
       
       if (!tree) return false;
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
                std::string data = callPluginExport(options.plugin, std::shared_ptr<ASTNode>(tree));
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