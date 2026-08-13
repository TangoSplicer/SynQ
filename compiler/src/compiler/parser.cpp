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
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "gate_validation.h"
#include "parser.h"

namespace {

std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    return first >= last ? "" : std::string(first, last);
}

bool is_non_negative_integer(const std::string& value) {
    return !value.empty() && std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

bool is_identifier(const std::string& value) {
    if (value.empty() || !(std::isalpha(static_cast<unsigned char>(value.front())) != 0 || value.front() == '_')) {
        return false;
    }
    return std::all_of(value.begin() + 1, value.end(), [](unsigned char ch) {
        return std::isalnum(ch) != 0 || ch == '_';
    });
}

bool is_qubit_operand(const std::string& value) {
    return value.size() > 3 && value.rfind("q[", 0) == 0 && value.back() == ']' &&
           is_non_negative_integer(value.substr(2, value.size() - 3));
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

bool is_parameter_expression(const std::string& value) {
    if (value == "pi" || value == "-pi" || is_decimal_parameter(value)) return true;
    const std::string prefix = value.rfind("-pi/", 0) == 0 ? "-pi/" : "pi/";
    return value.rfind(prefix, 0) == 0 && is_non_negative_integer(value.substr(prefix.size())) &&
           value.substr(prefix.size()) != "0";
}

bool is_integer_literal(const std::string& value) {
    const std::size_t start = !value.empty() && value.front() == '-' ? 1 : 0;
    if (start == value.size()) return false;
    return std::all_of(value.begin() + start, value.end(), [](unsigned char character) {
        return std::isdigit(character) != 0;
    });
}

ClassicalLiteralKind classify_declaration_literal(const std::string& value) {
    if (is_integer_literal(value)) return ClassicalLiteralKind::Integer;
    if (value.find('.') != std::string::npos && is_decimal_parameter(value)) return ClassicalLiteralKind::Decimal;
    if (value == "true" || value == "false") return ClassicalLiteralKind::Boolean;
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        return ClassicalLiteralKind::QuotedString;
    }
    return ClassicalLiteralKind::SourceText;
}

bool is_parameterized_kernel(const std::string& kernel) {
    const std::size_t open = kernel.find('(');
    if (open == std::string::npos) return is_identifier(kernel);
    if (kernel.back() != ')' || kernel.find('(', open + 1) != std::string::npos) return false;
    return is_identifier(kernel.substr(0, open)) && is_parameter_expression(kernel.substr(open + 1, kernel.size() - open - 2));
}

bool is_feature_name(const std::string& value) {
    if (value.empty()) return false;
    return std::all_of(value.begin(), value.end(), [](unsigned char character) {
        return std::islower(character) != 0 || std::isdigit(character) != 0 || character == '-';
    });
}

bool parse_experimental_feature_annotation(const std::string& line, std::string& feature_name) {
    const std::string prefix = "#[experimental(feature = \"";
    const std::string suffix = "\")]";
    if (line.rfind(prefix, 0) != 0 || line.size() <= prefix.size() + suffix.size() ||
        line.substr(line.size() - suffix.size()) != suffix) {
        return false;
    }
    feature_name = line.substr(prefix.size(), line.size() - prefix.size() - suffix.size());
    return is_feature_name(feature_name);
}

bool parse_quantum_arguments(const std::string& source, std::vector<std::string>& arguments) {
    std::istringstream tokens(source);
    std::string kernel;
    tokens >> kernel;
    if (kernel.empty() || !is_parameterized_kernel(kernel)) return false;
    arguments = {kernel};

    const bool has_parameter = kernel.find('(') != std::string::npos;
    std::string operands = trim(source.substr(kernel.size()));
    if (operands.empty()) return !has_parameter;  // Preserve only the documented legacy kernel form.

    std::size_t start = 0;
    while (start <= operands.size()) {
        const std::size_t comma = operands.find(',', start);
        const std::string operand = trim(operands.substr(start, comma == std::string::npos ? std::string::npos : comma - start));
        if (!is_qubit_operand(operand)) return false;
        arguments.push_back(operand);
        if (comma == std::string::npos) return !has_parameter || arguments.size() == 2;
        start = comma + 1;
    }
    return false;
}

bool parse_qubit_index(const std::string& operand, std::size_t& index) {
    if (!is_qubit_operand(operand)) return false;
    index = 0;
    for (std::size_t position = 2; position + 1 < operand.size(); ++position) {
        index = index * 10 + static_cast<std::size_t>(operand[position] - '0');
    }
    return true;
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

QuantumGateNode* make_quantum_gate_node(const std::vector<std::string>& arguments,
                                        std::size_t line_number,
                                        const synq::compiler::SourceSpan& span) {
    const std::string& kernel = arguments.front();
    const std::size_t open = kernel.find('(');
    const bool parameterized = open != std::string::npos;
    const std::string source_name = parameterized ? kernel.substr(0, open) : kernel;
    const std::optional<std::string> literal_angle = parameterized
        ? std::optional<std::string>(kernel.substr(open + 1, kernel.size() - open - 2))
        : std::nullopt;

    std::vector<std::size_t> operands;
    operands.reserve(arguments.size() - 1);
    for (std::size_t position = 1; position < arguments.size(); ++position) {
        std::size_t index = 0;
        if (!parse_qubit_index(arguments[position], index)) return nullptr;
        operands.push_back(index);
    }
    return new QuantumGateNode(quantum_gate_kind(source_name), source_name, literal_angle,
                               std::move(operands), line_number, span);
}

std::string strip_comment(const std::string& value) {
    // Recovery-profile rule: `//` begins a comment only at the start of a
    // trimmed line or when preceded by whitespace. This preserves source text
    // such as `https://example.invalid`.
    std::size_t marker = value.find("//");
    while (marker != std::string::npos) {
        if (marker == 0 || std::isspace(static_cast<unsigned char>(value[marker - 1])) != 0) {
            return trim(value.substr(0, marker));
        }
        marker = value.find("//", marker + 2);
    }
    return trim(value);
}

synq::compiler::SourceSpan span_for_line(const std::string& raw_line,
                                         const std::string& trimmed_line,
                                         std::size_t line_number) {
    const std::size_t first = raw_line.find_first_not_of(" \t\r\n");
    const std::size_t column_start = first == std::string::npos ? 1 : first + 1;
    return {line_number, column_start, column_start + trimmed_line.size()};
}

synq::compiler::ParseResult fail_parse(const std::string& code,
                                       const synq::compiler::SourceSpan& span,
                                       const std::string& message,
                                       const std::string& help) {
    synq::compiler::ParseResult result;
    result.diagnostics.push_back({code, synq::compiler::DiagnosticSeverity::Error, span, message, help});
    return result;
}

}  // namespace

Parser::Parser()
    : configured_features_(synq::compiler::make_default_feature_registry()) {}

bool Parser::enableExperimentalFeature(const std::string& feature_name) {
    return configured_features_.enable(feature_name);
}

synq::compiler::ParseResult Parser::parseFileWithDiagnostics(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        return fail_parse("SYNQ-P001", {}, "could not open source file", "verify the file path and read permission");
    }

    auto root = std::make_unique<ProgramNode>();
    synq::compiler::FeatureRegistry active_features = configured_features_;
    std::unordered_map<std::string, synq::compiler::SourceSpan> declared_names;
    std::string raw_line;
    std::size_t line_number = 0;
    while (std::getline(infile, raw_line)) {
        ++line_number;
        std::string line = strip_comment(trim(raw_line));
        if (line.empty()) continue;
        if (line.back() == ';') line = trim(line.substr(0, line.size() - 1));
        const synq::compiler::SourceSpan span = span_for_line(raw_line, line, line_number);

        if (line.rfind("#[", 0) == 0) {
            std::string feature_name;
            if (!parse_experimental_feature_annotation(line, feature_name) || !active_features.enable(feature_name)) {
                return fail_parse("SYNQ-P006", span, "unknown or malformed experimental feature annotation",
                                  "use a registered annotation such as #[experimental(feature = \"parameterized-quantum-gates\")]" );
            }
            continue;
        }

        std::istringstream tokens(line);
        std::string operation;
        tokens >> operation;
        std::string argument;
        std::getline(tokens, argument);
        argument = trim(argument);

        if (operation == "let") {
            const auto assignment = argument.find('=');
            const std::string identifier = assignment == std::string::npos ? "" : trim(argument.substr(0, assignment));
            const std::string value = assignment == std::string::npos ? "" : trim(argument.substr(assignment + 1));
            if (!is_identifier(identifier) || value.empty()) {
                return fail_parse("SYNQ-P002", span, "malformed declaration", "use let <identifier> = <value>");
            }
            const auto inserted = declared_names.emplace(identifier, span);
            if (!inserted.second) {
                return fail_parse("SYNQ-S004", span,
                                  "duplicate top-level declaration `" + identifier +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                  "rename the later binding or reuse the existing declaration according to future language semantics");
            }
            root->statements.push_back(new DeclarationNode(identifier, value, line_number,
                                                           classify_declaration_literal(value), span));
            continue;
        }

        const bool known_instruction = operation == "print" || operation == "delay" ||
                                       operation == "quantum" || operation == "ai";
        if (!known_instruction || argument.empty()) {
            return fail_parse("SYNQ-P003", span, "unsupported or incomplete recovery-profile instruction",
                              "use let, print, delay, quantum, or ai with the documented argument form");
        }
        if (operation == "delay" && !is_non_negative_integer(argument)) {
            return fail_parse("SYNQ-P004", span, "delay requires a non-negative integer number of milliseconds",
                              "use a non-negative whole number, such as delay 0");
        }

        if (operation == "quantum") {
            std::vector<std::string> quantum_arguments;
            if (!parse_quantum_arguments(argument, quantum_arguments)) {
                return fail_parse("SYNQ-P005", span, "malformed quantum kernel, operands, or literal-angle parameter",
                                  "use explicit operands such as q[0] or q[0], q[1]");
            }
            QuantumGateNode* gate = make_quantum_gate_node(quantum_arguments, line_number, span);
            if (gate == nullptr) {
                return fail_parse("SYNQ-P005", span, "could not construct typed quantum operands",
                                  "use explicit operands such as q[0] or q[0], q[1]");
            }
            const auto validation_error = synq::compiler::validate_quantum_gate_shape(*gate);
            if (validation_error.has_value()) {
                delete gate;
                return fail_parse(validation_error->code, span, validation_error->message, validation_error->help);
            }
            if (gate->literal_angle.has_value() &&
                !active_features.is_enabled("parameterized-quantum-gates")) {
                delete gate;
                return fail_parse("SYNQ-P007", span, "parameterized quantum gates require an alpha feature opt-in",
                                  "add #[experimental(feature = \"parameterized-quantum-gates\")] before the gated construct");
            }
            root->statements.push_back(gate);
        } else {
            root->statements.push_back(new InstructionNode(operation, {argument}, line_number, span));
        }
    }

    synq::compiler::ParseResult result;
    result.program = std::move(root);
    return result;
}

ASTNode* Parser::parseFile(const std::string& filename) {
    synq::compiler::ParseResult result = parseFileWithDiagnostics(filename);
    if (!result.ok()) {
        for (const synq::compiler::Diagnostic& diagnostic : result.diagnostics) {
            std::cerr << synq::compiler::format_diagnostic(filename, diagnostic) << std::endl;
        }
        return nullptr;
    }
    return result.take_program().release();
}
