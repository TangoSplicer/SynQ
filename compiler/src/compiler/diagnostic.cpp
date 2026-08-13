// Copyright (c) 2025 SynQ Contributors

#include "diagnostic.h"

#include <sstream>

namespace synq::compiler {

const char* diagnostic_severity_name(DiagnosticSeverity severity) {
    switch (severity) {
        case DiagnosticSeverity::Error:
            return "error";
        case DiagnosticSeverity::Warning:
            return "warning";
    }
    return "unknown";
}

std::string format_diagnostic(const std::string& source_name, const Diagnostic& diagnostic) {
    std::ostringstream output;
    output << source_name;
    if (diagnostic.span.line != 0) {
        output << ":" << diagnostic.span.line;
        if (diagnostic.span.column_start != 0) {
            output << ":" << diagnostic.span.column_start;
        }
    }
    output << ": " << diagnostic_severity_name(diagnostic.severity)
           << "[" << diagnostic.code << "]: " << diagnostic.message;
    if (!diagnostic.help.empty()) {
        output << "\n  help: " << diagnostic.help;
    }
    return output.str();
}

}  // namespace synq::compiler
