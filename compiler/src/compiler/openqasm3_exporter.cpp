#include "compiler/openqasm3_exporter.h"

#include <sstream>

namespace synq::compiler {
namespace {

void add_diagnostic(OpenQasm3ExportResult& result, std::size_t line, const std::string& message) {
    result.diagnostics.push_back("line " + std::to_string(line) + ": " + message);
}

}  // namespace

OpenQasm3ExportResult export_openqasm3(const ProgramNode& program) {
    OpenQasm3ExportResult result;
    std::ostringstream body;
    std::size_t qubit_count = 0;

    for (const ASTNode* statement : program.statements) {
        const auto* instruction = dynamic_cast<const InstructionNode*>(statement);
        if (instruction == nullptr) {
            const auto* declaration = dynamic_cast<const DeclarationNode*>(statement);
            add_diagnostic(result, declaration == nullptr ? 0 : declaration->line,
                           "only supported quantum instructions can be exported to OpenQASM 3");
            continue;
        }

        if (instruction->op != "quantum" || instruction->args.size() != 1) {
            add_diagnostic(result, instruction->line,
                           "only single-argument `quantum <kernel>` instructions can be exported to OpenQASM 3");
            continue;
        }

        const std::string& kernel = instruction->args.front();
        if (kernel == "h" || kernel == "x" || kernel == "y" || kernel == "z") {
            body << kernel << " q[0];\n";
            qubit_count = std::max<std::size_t>(qubit_count, 1);
        } else if (kernel == "bell_pair") {
            body << "h q[0];\n";
            body << "cx q[0], q[1];\n";
            qubit_count = std::max<std::size_t>(qubit_count, 2);
        } else {
            add_diagnostic(result, instruction->line,
                           "unsupported quantum kernel `" + kernel + "` for the OpenQASM 3 recovery exporter");
        }
    }

    if (!result.ok()) {
        return result;
    }
    if (qubit_count == 0) {
        add_diagnostic(result, 0, "no supported quantum instructions were available for OpenQASM 3 export");
        return result;
    }

    std::ostringstream output;
    output << "OPENQASM 3.0;\n";
    output << "include \"stdgates.inc\";\n";
    output << "qubit[" << qubit_count << "] q;\n";
    output << body.str();
    result.program = output.str();
    return result;
}

}  // namespace synq::compiler
