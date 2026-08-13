// Copyright (c) 2025 SynQ Contributors
//
// Typed diagnostic data for the recovery-profile compiler front end.
#ifndef SYNQ_COMPILER_DIAGNOSTIC_H
#define SYNQ_COMPILER_DIAGNOSTIC_H

#include <cstddef>
#include <string>

namespace synq::compiler {

struct SourceSpan {
    // A zero line and columns denote an unknown location, such as a file that
    // could not be opened. Otherwise locations are one-based and end-exclusive.
    std::size_t line = 0;
    std::size_t column_start = 0;
    std::size_t column_end = 0;
};

enum class DiagnosticSeverity {
    Error,
    Warning,
};

struct Diagnostic {
    std::string code;
    DiagnosticSeverity severity = DiagnosticSeverity::Error;
    SourceSpan span;
    std::string message;
    std::string help;
};

const char* diagnostic_severity_name(DiagnosticSeverity severity);
std::string format_diagnostic(const std::string& source_name, const Diagnostic& diagnostic);

}  // namespace synq::compiler

#endif
