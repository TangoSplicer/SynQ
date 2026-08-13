#include "compiler/openqasm3_exporter.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <memory>
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

bool is_decimal_parameter(const std::string& value) {
    std::size_t position = !value.empty() && value.front() == '-' ? 1 : 0;
    if (position == value.size()) return false;
    bool has_digit = false;
    bool has_decimal_point = false;
    for (; position < value.size(); ++position) {
        const unsigned char character = static_cast<unsigned char>(value[position]);
        if (std::isdigit(character) != 0) {
            has_digit = true;
        } else if (value[position] == '.' && !has_decimal_point) {
            has_decimal_point = true;
        } else {
            return false;
        }
    }
    return has_digit;
}

bool is_literal_angle_parameter(const std::string& value) {
    if (value == "pi" || value == "-pi" || is_decimal_parameter(value)) return true;
    const std::string prefix = value.rfind("-pi/", 0) == 0 ? "-pi/" : "pi/";
    const std::string denominator = value.rfind(prefix, 0) == 0 ? value.substr(prefix.size()) : "";
    return !denominator.empty() && denominator != "0" &&
           std::all_of(denominator.begin(), denominator.end(), [](unsigned char character) {
               return std::isdigit(character) != 0;
           });
}

bool split_parameterized_kernel(const std::string& kernel, std::string& gate, std::string& parameter) {
    const std::size_t open = kernel.find('(');
    if (open == std::string::npos || kernel.back() != ')' || kernel.find('(', open + 1) != std::string::npos) return false;
    gate = kernel.substr(0, open);
    parameter = kernel.substr(open + 1, kernel.size() - open - 2);
    return !gate.empty() && is_literal_angle_parameter(parameter);
}

void write_parameterized_single_qubit_gate(
    std::ostringstream& body, const std::string& gate, const std::string& parameter, std::size_t qubit) {
    body << gate << "(" << parameter << ") q[" << qubit << "];\n";
}

QuantumGateKind quantum_gate_kind(const std::string& source_name) {
    if (source_name == "h") return QuantumGateKind::H;
    if (source_name == "x") return QuantumGateKind::X;
    if (source_name == "y") return QuantumGateKind::Y;
    if (source_name == "z") return QuantumGateKind::Z;
    if (source_name == "cx") return QuantumGateKind::Cx;
    if (source_name == "bell_pair") return QuantumGateKind::BellPair;
    if (source_name == "rx") return QuantumGateKind::Rx;
    if (source_name == "ry") return QuantumGateKind::Ry;
    if (source_name == "rz") return QuantumGateKind::Rz;
    if (source_name == "p") return QuantumGateKind::Phase;
    return QuantumGateKind::Unknown;
}

std::unique_ptr<QuantumGateNode> typed_legacy_instruction(const InstructionNode& instruction) {
    if (instruction.op != "quantum" || instruction.args.empty()) return nullptr;
    std::vector<std::size_t> operands;
    if (!parse_operands(instruction.args, operands)) return nullptr;

    const std::string& kernel = instruction.args.front();
    std::string source_name;
    std::string parameter;
    if (split_parameterized_kernel(kernel, source_name, parameter)) {
        return std::make_unique<QuantumGateNode>(quantum_gate_kind(source_name), source_name,
                                                 parameter, std::move(operands), instruction.line);
    }
    return std::make_unique<QuantumGateNode>(quantum_gate_kind(kernel), kernel, std::nullopt,
                                             std::move(operands), instruction.line);
}

void lower_quantum_gate(const QuantumGateNode& gate, std::ostringstream& body,
                        std::size_t& qubit_count, OpenQasm3ExportResult& result) {
    if (gate.kind == QuantumGateKind::H || gate.kind == QuantumGateKind::X ||
        gate.kind == QuantumGateKind::Y || gate.kind == QuantumGateKind::Z) {
        if (gate.literal_angle.has_value()) {
            add_diagnostic(result, gate.line,
                           "unsupported parameterized gate `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
            return;
        }
        if (gate.qubit_indices.size() > 1) {
            add_diagnostic(result, gate.line, "single-qubit kernel `" + gate.source_name + "` accepts at most one operand");
            return;
        }
        const std::size_t qubit = gate.qubit_indices.empty() ? 0 : gate.qubit_indices.front();
        write_single_qubit_gate(body, gate.source_name, qubit);
        qubit_count = std::max(qubit_count, qubit + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::Cx) {
        if (gate.literal_angle.has_value() || gate.qubit_indices.size() != 2) {
            add_diagnostic(result, gate.line, "kernel `cx` requires exactly two explicit qubit operands");
            return;
        }
        body << "cx q[" << gate.qubit_indices[0] << "], q[" << gate.qubit_indices[1] << "];\n";
        qubit_count = std::max(qubit_count, std::max(gate.qubit_indices[0], gate.qubit_indices[1]) + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::BellPair) {
        if (gate.literal_angle.has_value() || (gate.qubit_indices.size() != 0 && gate.qubit_indices.size() != 2)) {
            add_diagnostic(result, gate.line, "kernel `bell_pair` accepts zero or exactly two explicit qubit operands");
            return;
        }
        const std::size_t first = gate.qubit_indices.empty() ? 0 : gate.qubit_indices[0];
        const std::size_t second = gate.qubit_indices.empty() ? 1 : gate.qubit_indices[1];
        write_single_qubit_gate(body, "h", first);
        body << "cx q[" << first << "], q[" << second << "];\n";
        qubit_count = std::max(qubit_count, std::max(first, second) + 1);
        return;
    }

    if (gate.kind == QuantumGateKind::Rx || gate.kind == QuantumGateKind::Ry ||
        gate.kind == QuantumGateKind::Rz || gate.kind == QuantumGateKind::Phase) {
        if (!gate.literal_angle.has_value() || gate.qubit_indices.size() != 1) {
            add_diagnostic(result, gate.line,
                           "parameterized gate `" + gate.source_name + "` requires exactly one explicit qubit operand");
            return;
        }
        write_parameterized_single_qubit_gate(body, gate.source_name, *gate.literal_angle, gate.qubit_indices.front());
        qubit_count = std::max(qubit_count, gate.qubit_indices.front() + 1);
        return;
    }

    if (gate.literal_angle.has_value()) {
        add_diagnostic(result, gate.line,
                       "unsupported parameterized gate `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
    } else {
        add_diagnostic(result, gate.line,
                       "unsupported quantum kernel `" + gate.source_name + "` for the OpenQASM 3 recovery exporter");
    }
}

}  // namespace

OpenQasm3ExportResult export_openqasm3(const ProgramNode& program) {
    OpenQasm3ExportResult result;
    std::ostringstream body;
    std::size_t qubit_count = 0;

    for (const ASTNode* statement : program.statements) {
        const auto* typed_gate = dynamic_cast<const QuantumGateNode*>(statement);
        std::unique_ptr<QuantumGateNode> legacy_gate;
        if (typed_gate == nullptr) {
            const auto* instruction = dynamic_cast<const InstructionNode*>(statement);
            if (instruction != nullptr) {
                if (instruction->op != "quantum" || instruction->args.empty()) {
                    add_diagnostic(result, instruction->line,
                                   "only `quantum <kernel> [q[index](, q[index])*]` instructions can be exported to OpenQASM 3");
                    continue;
                }
                legacy_gate = typed_legacy_instruction(*instruction);
                if (legacy_gate == nullptr) {
                    add_diagnostic(result, instruction->line,
                                   "OpenQASM 3 export requires explicit operands in the form `q[index]`");
                    continue;
                }
                typed_gate = legacy_gate.get();
            }
        }
        if (typed_gate == nullptr) {
            const auto* declaration = dynamic_cast<const DeclarationNode*>(statement);
            add_diagnostic(result, declaration == nullptr ? 0 : declaration->line,
                           "only supported quantum instructions can be exported to OpenQASM 3");
            continue;
        }
        lower_quantum_gate(*typed_gate, body, qubit_count, result);
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
