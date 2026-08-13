#ifndef SYNQ_COMPILER_OPENQASM3_EXPORTER_H
#define SYNQ_COMPILER_OPENQASM3_EXPORTER_H

#include <string>
#include <vector>

#include "compiler/ast.h"

namespace synq::compiler {

// Result of exporting the small recovery-profile quantum subset to OpenQASM 3.
// A non-empty diagnostics collection means `program` must not be used.
struct OpenQasm3ExportResult {
    std::string program;
    std::vector<std::string> diagnostics;

    bool ok() const { return diagnostics.empty(); }
};

// Export only `quantum h`, `quantum x`, `quantum y`, `quantum z`, and
// `quantum bell_pair` instructions from a ProgramNode. Every other recovery
// statement is rejected instead of being ignored or guessed at.
OpenQasm3ExportResult export_openqasm3(const ProgramNode& program);

}  // namespace synq::compiler

#endif
