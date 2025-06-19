// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

#include "compiler/runtime.h"
#include "compiler/exporter.h"
#include "compiler/plugin_loader.h"
#include "compiler/debug_hooks.h"

namespace fs = std::filesystem;

void show_help() {
    std::cout << "🧠 SynQ CLI Compiler\n"
              << "Usage:\n"
              << "  synqc <file.synq> [--run] [--export] [--target=...] [--repl] [--dry-run] [--out=dir]\n\n"
              << "Flags:\n"
              << "  --run          Execute the file immediately\n"
              << "  --export       Emit backend artifact for the file\n"
              << "  --target=...   Choose target: x86, wasm, qiskit, synqvm\n"
              << "  --out=dir      Specify output path\n"
              << "  --dry-run      Show output location without saving file\n"
              << "  --repl         Launch interactive REPL (experimental)\n"
              << std::endl;
}

std::string load_file(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        throw std::runtime_error("❌ Cannot open file: " + path);
    }
    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

int main(int argc, char** argv) {
    if (argc < 2) {
        show_help();
        return 1;
    }

    std::string filepath = argv[1];
    bool do_run = false;
    bool do_export = false;
    bool do_repl = false;

    ExportOptions export_opts;
    export_opts.outputDir = "";
    export_opts.dryRun = false;
    export_opts.target = ExportTarget::UNKNOWN;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--run") do_run = true;
        else if (arg == "--export") do_export = true;
        else if (arg.starts_with("--target=")) {
            std::string tgt = arg.substr(9);
            export_opts.target = Exporter().parseTarget(tgt);
        }
        else if (arg == "--dry-run") export_opts.dryRun = true;
        else if (arg == "--repl") do_repl = true;
        else if (arg.starts_with("--out=")) {
            export_opts.outputDir = arg.substr(6);
        }
    }

    try {
        std::string code = load_file(filepath);
        std::cout << "📄 Loaded: " << filepath << std::endl;

        if (do_export) {
            if (export_opts.target == ExportTarget::UNKNOWN) {
                std::cerr << "❌ Export failed: --target must be specified.\n";
                return 2;
            }
            Exporter exporter;
            std::string compiled = compileToTarget(code, export_opts.target);
            exporter.exportCode(filepath, compiled, export_opts);
        }

        if (do_run) {
            Runtime runtime;
            runtime.execute(filepath, code);
        }

        if (do_repl) {
            Runtime runtime;
            runtime.repl();
        }

    } catch (const std::exception& ex) {
        std::cerr << "❌ Error: " << ex.what() << std::endl;
        return 3;
    }

    return 0;
}