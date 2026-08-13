#include "compiler/openqasm3_exporter.h"

#include <algorithm>
#include <limits>
#include <sstream>

namespace synq::compiler {
namespace {

void add_diagnostic(OpenQasm3ExportResult& result, std::size_t line, const std::string& message) {
    result.diagnostics.push_back("line " + std::to_string(line) + ": " + message);
}

bool parse_qubit_operand(const std::string& operand, std::size_t& index) {
    if (operand.size() <= 3 || operand.rfind("q[", 0) != 0 || operand.back() != ']') return false;
    index = 0;
    for (std::size_t position = 2; position + 1 < operand.size(); ++position) {
        const char character = operand[position];
        if (character < '0' || character > '9') return false;
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (index > (std::numeric_limits<std::size_t>::max() - digit) / 10) return false;
        index = index * 10 + digit;
    }
    return true;
}

bool parse_operands(const std::vector<std::string>& arguments, std::vector<std::size_t>& operands) {
    operands.clear();
    for (std::size_t position = 1; position < arguments.size(); ++position) {
        std::size_t index = 0;
        if (!parse_qubit_operand(arguments[position], index)) return false;
        operands.push_back(index);
    }
    return true;
}

void write_single_qubit_gate(std::ostringstream& body, const std::string& gate, std::size_t qubit) {
    body << gate << " q[" << qubit << "];\n";
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

        if (instruction->op != "quantum" || instruction->args.empty()) {
            add_diagnostic(result, instruction->line,
                           "only `quantum <kernel> [q[index](, q[index])*]` instructions can be exported to OpenQASM 3");
            continue;
        }

        const std::string& kernel = instruction->args.front();
        std::vector<std::size_t> operands;
        if (!parse_operands(instruction->args, operands)) {
            add_diagnostic(result, instruction->line,
                           "OpenQASM 3 export requires explicit operands in the form `q[index]`");
            continue;
        }
        if (kernel == "h" || kernel == "x" || kernel == "y" || kernel == "z") {
            if (operands.size() > 1) {
                add_diagnostic(result, instruction->line, "single-qubit kernel `" + kernel + "` accepts at most one operand");
                continue;
            }
            const std::size_t qubit = operands.empty() ? 0 : operands.front();
            write_single_qubit_gate(body, kernel, qubit);
            qubit_count = std::max(qubit_count, qubit + 1);
        } else if (kernel == "cx") {
            if (operands.size() != 2) {
                add_diagnostic(result, instruction->line, "kernel `cx` requires exactly two explicit qubit operands");
                continue;
            }
            body << "cx q[" << operands[0] << "], q[" << operands[1] << "];\n";
            qubit_count = std::max(qubit_count, std::max(operands[0], operands[1]) + 1);
        } else if (kernel == "bell_pair") {
            if (operands.size() != 0 && operands.size() != 2) {
                add_diagnostic(result, instruction->line, "kernel `bell_pair` accepts zero or exactly two explicit qubit operands");
                continue;
            }
            const std::size_t first = operands.empty() ? 0 : operands[0];
            const std::size_t second = operands.empty() ? 1 : operands[1];
            write_single_qubit_gate(body, "h", first);
            body << "cx q[" << first << "], q[" << second << "];\n";
            qubit_count = std::max(qubit_count, std::max(first, second) + 1);
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
