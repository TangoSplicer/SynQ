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

#include "classical_expression.h"
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
    const std::size_t open = value.find('[');
    return open != std::string::npos && open > 0 && value.back() == ']' &&
           value.find('[', open + 1) == std::string::npos &&
           is_identifier(value.substr(0, open)) &&
           is_non_negative_integer(value.substr(open + 1, value.size() - open - 2));
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

bool classify_bounded_classical_value(const std::string& value,
                                      const synq::compiler::FeatureRegistry& active_features,
                                      const synq::compiler::SourceSpan& span,
                                      ClassicalLiteralKind& declaration_kind,
                                      std::string& message,
                                      std::string& help) {
    declaration_kind = classify_declaration_literal(value);
    if (active_features.is_enabled("integer-arithmetic-expressions") &&
        synq::compiler::looks_like_integer_arithmetic_expression(value)) {
        ClassicalIntegerArithmeticExpression arithmetic;
        if (!synq::compiler::parse_bounded_integer_arithmetic_expression(value, span, arithmetic)) {
            message = "malformed bounded integer arithmetic expression";
            help = "use exactly <integer-literal-or-identifier> +, -, or * <integer-literal-or-identifier>";
            return false;
        }
        declaration_kind = ClassicalLiteralKind::IntegerArithmeticExpression;
    }
    if (active_features.is_enabled("classical-control-flow") &&
        synq::compiler::looks_like_boolean_expression(value)) {
        ClassicalBooleanExpression boolean_expression;
        if (!synq::compiler::parse_bounded_boolean_declaration_expression(value, span, boolean_expression)) {
            message = "malformed bounded Boolean expression";
            help = "use exactly not <Boolean-literal-or-identifier> or <Boolean-literal-or-identifier> and/or <Boolean-literal-or-identifier>";
            return false;
        }
        declaration_kind = ClassicalLiteralKind::BooleanExpression;
    }
    return true;
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

bool parse_qubit_operand(const std::string& operand, std::string& register_name, std::size_t& index) {
    if (!is_qubit_operand(operand)) return false;
    const std::size_t open = operand.find('[');
    register_name = operand.substr(0, open);
    index = 0;
    for (std::size_t position = open + 1; position + 1 < operand.size(); ++position) {
        index = index * 10 + static_cast<std::size_t>(operand[position] - '0');
    }
    return true;
}

bool parse_measurement_arguments(const std::string& source,
                                 std::string& register_name,
                                 std::size_t& qubit_index,
                                 std::optional<std::string>& result_name) {
    const std::string separator = " as ";
    const std::size_t boundary = source.find(separator);
    if (boundary == std::string::npos) {
        result_name = std::nullopt;
        return parse_qubit_operand(source, register_name, qubit_index);
    }
    if (source.find(separator, boundary + separator.size()) != std::string::npos) return false;
    const std::string operand = trim(source.substr(0, boundary));
    const std::string name = trim(source.substr(boundary + separator.size()));
    if (!parse_qubit_operand(operand, register_name, qubit_index) || !is_identifier(name)) return false;
    result_name = name;
    return true;
}

bool parse_qubit_declaration(const std::string& source, std::string& name, std::size_t& qubit_count) {
    const std::size_t open = source.find('[');
    if (open == std::string::npos || source.empty() || source.back() != ']' ||
        source.find('[', open + 1) != std::string::npos) {
        return false;
    }
    const std::string size_text = source.substr(open + 1, source.size() - open - 2);
    name = source.substr(0, open);
    if (!is_identifier(name) || !is_non_negative_integer(size_text) || size_text == "0") return false;

    qubit_count = 0;
    for (char character : size_text) {
        qubit_count = qubit_count * 10 + static_cast<std::size_t>(character - '0');
    }
    return true;
}

bool parse_callable_declaration(const std::string& source, std::string& name) {
    if (source.size() < 3 || source.substr(source.size() - 2) != "()") return false;
    name = source.substr(0, source.size() - 2);
    return is_identifier(name);
}

QuantumGateKind quantum_gate_kind(const std::string& source_name);

bool parse_bounded_kernel_body(const std::string& source, std::string& name, std::string& body) {
    const std::size_t open = source.find("() {");
    if (open == std::string::npos || source.size() <= open + 6 || source.back() != '}') return false;
    name = trim(source.substr(0, open));
    body = trim(source.substr(open + 4, source.size() - open - 5));
    return is_identifier(name) && body.rfind("quantum ", 0) == 0 && body.find(';') == std::string::npos;
}

bool split_comma_list(const std::string& source, std::vector<std::string>& values) {
    values.clear();
    std::size_t start = 0;
    while (start <= source.size()) {
        const std::size_t comma = source.find(',', start);
        const std::string value = trim(source.substr(start, comma == std::string::npos ? std::string::npos : comma - start));
        if (value.empty()) return false;
        values.push_back(value);
        if (comma == std::string::npos) return true;
        start = comma + 1;
    }
    return false;
}

bool parse_parameterized_routine_call(const std::string& source, std::string& name,
                                      std::vector<std::string>& actuals) {
    const std::size_t open = source.find('(');
    if (open == std::string::npos || source.back() != ')' || open == 0) return false;
    name = trim(source.substr(0, open));
    return is_identifier(name) && split_comma_list(source.substr(open + 1, source.size() - open - 2), actuals);
}

bool looks_like_parameterized_routine_call(const std::string& source) {
    const std::size_t open = source.find('(');
    return open != std::string::npos && source.back() == ')' &&
           !trim(source.substr(open + 1, source.size() - open - 2)).empty();
}

bool looks_like_parameterized_routine_declaration(const std::string& source) {
    const std::size_t open = source.find('(');
    const std::size_t close = source.find(')', open == std::string::npos ? 0 : open + 1);
    return open != std::string::npos && close != std::string::npos && close > open &&
           !trim(source.substr(open + 1, close - open - 1)).empty();
}

bool parse_parameterized_routine(const std::string& source, std::string& name,
                                 std::vector<RoutineFormal>& formals,
                                 ParameterizedRoutineBody& body,
                                 const synq::compiler::SourceSpan& span) {
    const std::size_t open = source.find('(');
    const std::size_t close = source.find(')', open == std::string::npos ? 0 : open + 1);
    if (open == std::string::npos || close == std::string::npos || close <= open || source.back() != '}') return false;
    name = trim(source.substr(0, open));
    const std::string suffix = trim(source.substr(close + 1));
    if (!is_identifier(name) || suffix.rfind("{ quantum ", 0) != 0) return false;
    std::vector<std::string> formal_tokens;
    if (!split_comma_list(source.substr(open + 1, close - open - 1), formal_tokens)) return false;
    formals.clear();
    for (const std::string& token : formal_tokens) {
        std::istringstream words(token);
        std::string type;
        std::string identifier;
        std::string extra;
        words >> type >> identifier >> extra;
        if (!extra.empty() || !is_identifier(identifier) ||
            std::any_of(formals.begin(), formals.end(), [&identifier](const RoutineFormal& formal) { return formal.name == identifier; })) return false;
        if (type == "angle") {
            if (std::any_of(formals.begin(), formals.end(), [](const RoutineFormal& formal) { return formal.kind == RoutineFormalKind::Angle; })) return false;
            formals.push_back({RoutineFormalKind::Angle, identifier});
        } else if (type == "qubit") {
            formals.push_back({RoutineFormalKind::Qubit, identifier});
        } else {
            return false;
        }
    }
    const std::string gate_source = trim(suffix.substr(10, suffix.size() - 11));
    std::istringstream gate_words(gate_source);
    std::string kernel;
    gate_words >> kernel;
    std::string operands;
    std::getline(gate_words, operands);
    operands = trim(operands);
    if (kernel.empty() || operands.empty()) return false;
    const std::size_t angle_open = kernel.find('(');
    const bool has_angle = angle_open != std::string::npos;
    const std::string gate_name = has_angle ? kernel.substr(0, angle_open) : kernel;
    std::optional<std::string> angle;
    if (has_angle) {
        if (kernel.back() != ')' || kernel.find('(', angle_open + 1) != std::string::npos) return false;
        angle = kernel.substr(angle_open + 1, kernel.size() - angle_open - 2);
        if (!is_identifier(*angle)) return false;
    }
    std::vector<std::string> qubits;
    if (!split_comma_list(operands, qubits)) return false;
    const auto formal_named = [&formals](const std::string& candidate, RoutineFormalKind kind) {
        return std::any_of(formals.begin(), formals.end(), [&candidate, kind](const RoutineFormal& formal) {
            return formal.kind == kind && formal.name == candidate;
        });
    };
    const QuantumGateKind kind = quantum_gate_kind(gate_name);
    const bool valid_parameterized = has_angle &&
        (kind == QuantumGateKind::Rx || kind == QuantumGateKind::Ry || kind == QuantumGateKind::Rz || kind == QuantumGateKind::Phase) &&
        formal_named(*angle, RoutineFormalKind::Angle) && qubits.size() == 1 && formal_named(qubits[0], RoutineFormalKind::Qubit);
    const bool valid_fixed = !has_angle &&
        (kind == QuantumGateKind::H || kind == QuantumGateKind::X || kind == QuantumGateKind::Y || kind == QuantumGateKind::Z) &&
        qubits.size() == 1 && formal_named(qubits[0], RoutineFormalKind::Qubit);
    const bool valid_cx = !has_angle && kind == QuantumGateKind::Cx && qubits.size() == 2 && qubits[0] != qubits[1] &&
        formal_named(qubits[0], RoutineFormalKind::Qubit) && formal_named(qubits[1], RoutineFormalKind::Qubit);
    if (!valid_parameterized && !valid_fixed && !valid_cx) return false;
    body = {kind, gate_name, angle, qubits, span};
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
    std::vector<std::string> register_names;
    operands.reserve(arguments.size() - 1);
    for (std::size_t position = 1; position < arguments.size(); ++position) {
        std::size_t index = 0;
        std::string register_name;
        if (!parse_qubit_operand(arguments[position], register_name, index)) return nullptr;
        operands.push_back(index);
        register_names.push_back(std::move(register_name));
    }
    return new QuantumGateNode(quantum_gate_kind(source_name), source_name, literal_angle,
                               std::move(operands), line_number, span, std::move(register_names));
}

bool uses_named_register_operand(const QuantumGateNode& gate) {
    return std::any_of(gate.qubit_register_names.begin(), gate.qubit_register_names.end(),
                       [](const std::string& name) { return name != "q"; });
}

bool parse_boolean_expression_atom(const std::string& source,
                                   const synq::compiler::SourceSpan& span,
                                   ClassicalBooleanExpression& expression) {
    if (source == "true" || source == "false") {
        expression = {ClassicalBooleanExpressionKind::BooleanLiteral, source == "true", source, span, {}};
        return true;
    }
    if (is_identifier(source)) {
        expression = {ClassicalBooleanExpressionKind::IdentifierReference, false, source, span, {}};
        return true;
    }
    return false;
}

bool parse_bounded_boolean_expression(const std::string& source,
                                      const synq::compiler::SourceSpan& span,
                                      ClassicalBooleanExpression& expression) {
    std::istringstream tokens(source);
    std::vector<std::string> words;
    std::string word;
    while (tokens >> word) words.push_back(word);

    if (words.size() == 1) return parse_boolean_expression_atom(words.front(), span, expression);

    if (words.size() == 2 && words.front() == "not") {
        ClassicalBooleanExpression operand;
        if (!parse_boolean_expression_atom(words[1], span, operand)) return false;
        expression = {ClassicalBooleanExpressionKind::Not, false, source, span, {std::move(operand)}};
        return true;
    }

    if (words.size() == 3 && (words[1] == "and" || words[1] == "or")) {
        ClassicalBooleanExpression left;
        ClassicalBooleanExpression right;
        if (!parse_boolean_expression_atom(words[0], span, left) ||
            !parse_boolean_expression_atom(words[2], span, right)) {
            return false;
        }
        expression = {words[1] == "and" ? ClassicalBooleanExpressionKind::And
                                          : ClassicalBooleanExpressionKind::Or,
                      false, source, span, {std::move(left), std::move(right)}};
        return true;
    }

    return false;
}

bool parse_control_flow_arguments(const std::string& source, const std::string& connector,
                                  ClassicalCondition& condition, const synq::compiler::SourceSpan& span,
                                  std::string& body_operation, std::string& body_argument) {
    const std::string separator = " " + connector + " ";
    const std::size_t boundary = source.find(separator);
    if (boundary == std::string::npos || source.find(separator, boundary + separator.size()) != std::string::npos) {
        return false;
    }

    const std::string condition_text = trim(source.substr(0, boundary));
    ClassicalBooleanExpression expression;
    if (!parse_bounded_boolean_expression(condition_text, span, expression)) return false;
    ClassicalConditionKind condition_kind = ClassicalConditionKind::BooleanExpression;
    if (expression.kind == ClassicalBooleanExpressionKind::BooleanLiteral) {
        condition_kind = ClassicalConditionKind::BooleanLiteral;
    } else if (expression.kind == ClassicalBooleanExpressionKind::IdentifierReference) {
        condition_kind = ClassicalConditionKind::IdentifierReference;
    }
    condition = {condition_kind, expression.boolean_value, condition_text, span, std::move(expression)};

    std::istringstream body_tokens(trim(source.substr(boundary + separator.size())));
    body_tokens >> body_operation;
    std::getline(body_tokens, body_argument);
    body_argument = trim(body_argument);
    return !body_operation.empty() && !body_argument.empty();
}

ASTNode* make_control_body_node(const std::string& operation, const std::string& argument,
                                const synq::compiler::FeatureRegistry& active_features,
                                std::size_t line_number, const synq::compiler::SourceSpan& span,
                                synq::compiler::Diagnostic& error) {
    if (operation == "quantum") {
        std::vector<std::string> quantum_arguments;
        if (!parse_quantum_arguments(argument, quantum_arguments)) {
            error = {"SYNQ-P010", synq::compiler::DiagnosticSeverity::Error, span,
                     "classical control-flow body has malformed quantum syntax",
                     "use one bounded quantum statement such as quantum h q[0]"};
            return nullptr;
        }
        QuantumGateNode* gate = make_quantum_gate_node(quantum_arguments, line_number, span);
        if (gate == nullptr) {
            error = {"SYNQ-P010", synq::compiler::DiagnosticSeverity::Error, span,
                     "classical control-flow body could not construct typed quantum operands",
                     "use explicit operands such as q[0] or q[0], q[1]"};
            return nullptr;
        }
        const auto validation_error = synq::compiler::validate_quantum_gate_shape(*gate);
        if (validation_error.has_value()) {
            error = {validation_error->code, synq::compiler::DiagnosticSeverity::Error, span,
                     validation_error->message, validation_error->help};
            delete gate;
            return nullptr;
        }
        if (gate->literal_angle.has_value() && !active_features.is_enabled("parameterized-quantum-gates")) {
            error = {"SYNQ-P007", synq::compiler::DiagnosticSeverity::Error, span,
                     "parameterized quantum gates require an alpha feature opt-in",
                     "add #[experimental(feature = \"parameterized-quantum-gates\")] before the gated construct"};
            delete gate;
            return nullptr;
        }
        if (uses_named_register_operand(*gate) && !active_features.is_enabled("named-qubit-register-operands")) {
            error = {"SYNQ-P007", synq::compiler::DiagnosticSeverity::Error, span,
                     "named qubit register operands require an alpha feature opt-in",
                     "add #[experimental(feature = \"named-qubit-register-operands\")] before the gated construct"};
            delete gate;
            return nullptr;
        }
        return gate;
    }

    if (operation == "measure") {
        std::string register_name;
        std::size_t qubit_index = 0;
        if (!parse_qubit_operand(argument, register_name, qubit_index)) {
            error = {"SYNQ-P008", synq::compiler::DiagnosticSeverity::Error, span,
                     "measurement requires exactly one explicit qubit operand",
                     "use measure q[index], for example measure q[0]"};
            return nullptr;
        }
        if (register_name != "q" && !active_features.is_enabled("named-qubit-register-operands")) {
            error = {"SYNQ-P007", synq::compiler::DiagnosticSeverity::Error, span,
                     "named qubit register operands require an alpha feature opt-in",
                     "add #[experimental(feature = \"named-qubit-register-operands\")] before the gated construct"};
            return nullptr;
        }
        return new MeasurementNode(qubit_index, line_number, span, std::nullopt, std::move(register_name));
    }

    error = {"SYNQ-P010", synq::compiler::DiagnosticSeverity::Error, span,
             "classical control-flow body must be one typed quantum gate or measurement",
             "use quantum <gate> q[index] or measure q[index] as the single body statement"};
    return nullptr;
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

synq::compiler::ParseResult Parser::parseStreamWithDiagnostics(std::istream& input) {
    auto root = std::make_unique<ProgramNode>();
    synq::compiler::FeatureRegistry active_features = configured_features_;
    std::unordered_map<std::string, synq::compiler::SourceSpan> declared_names;
    std::string raw_line;
    std::size_t line_number = 0;
    while (std::getline(input, raw_line)) {
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
            ClassicalLiteralKind declaration_kind;
            std::string message;
            std::string help;
            if (!classify_bounded_classical_value(value, active_features, span, declaration_kind, message, help)) {
                return fail_parse("SYNQ-P011", span, message, help);
            }
            const auto inserted = declared_names.emplace(identifier, span);
            if (!inserted.second) {
                return fail_parse("SYNQ-S004", span,
                                  "duplicate top-level declaration `" + identifier +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                  "rename the later binding or reuse the existing declaration according to future language semantics");
            }
            root->statements.push_back(new DeclarationNode(identifier, value, line_number, declaration_kind, span));
            continue;
        }

        if (operation == "var") {
            if (!active_features.is_enabled("mutable-classical-state")) {
                return fail_parse("SYNQ-P014", span, "mutable declarations require an alpha feature opt-in",
                                  "add #[experimental(feature = \"mutable-classical-state\")] before var or set");
            }
            const auto assignment = argument.find('=');
            const std::string identifier = assignment == std::string::npos ? "" : trim(argument.substr(0, assignment));
            const std::string value = assignment == std::string::npos ? "" : trim(argument.substr(assignment + 1));
            if (!is_identifier(identifier) || value.empty()) {
                return fail_parse("SYNQ-P015", span, "malformed mutable declaration",
                                  "use var <identifier> = <supported-expression>");
            }
            ClassicalLiteralKind declaration_kind;
            std::string message;
            std::string help;
            if (!classify_bounded_classical_value(value, active_features, span, declaration_kind, message, help)) {
                return fail_parse("SYNQ-P011", span, message, help);
            }
            const auto inserted = declared_names.emplace(identifier, span);
            if (!inserted.second) {
                return fail_parse("SYNQ-S004", span,
                                  "duplicate top-level declaration `" + identifier +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                  "rename the mutable cell or reuse the existing declaration according to the bounded state contract");
            }
            root->statements.push_back(new MutableDeclarationNode(identifier, value, line_number, declaration_kind, span));
            continue;
        }

        if (operation == "set") {
            if (!active_features.is_enabled("mutable-classical-state")) {
                return fail_parse("SYNQ-P014", span, "mutable assignments require an alpha feature opt-in",
                                  "add #[experimental(feature = \"mutable-classical-state\")] before var or set");
            }
            const auto assignment = argument.find('=');
            const std::string identifier = assignment == std::string::npos ? "" : trim(argument.substr(0, assignment));
            const std::string value = assignment == std::string::npos ? "" : trim(argument.substr(assignment + 1));
            if (!is_identifier(identifier) || value.empty()) {
                return fail_parse("SYNQ-P016", span, "malformed mutable assignment",
                                  "use set <earlier-mutable-identifier> = <supported-expression>");
            }
            ClassicalLiteralKind declaration_kind;
            std::string message;
            std::string help;
            if (!classify_bounded_classical_value(value, active_features, span, declaration_kind, message, help)) {
                return fail_parse("SYNQ-P011", span, message, help);
            }
            root->statements.push_back(new AssignmentNode(identifier, value, line_number, declaration_kind, span));
            continue;
        }

        if (operation == "qubit") {
            if (!active_features.is_enabled("qubit-declarations")) {
                return fail_parse("SYNQ-P007", span, "qubit declarations require an alpha feature opt-in",
                                  "add #[experimental(feature = \"qubit-declarations\")] before the gated construct");
            }
            std::string name;
            std::size_t qubit_count = 0;
            if (!parse_qubit_declaration(argument, name, qubit_count)) {
                return fail_parse("SYNQ-P012", span, "malformed bounded qubit declaration",
                                  "use qubit <identifier>[positive-size], for example qubit q[2]");
            }
            const auto inserted = declared_names.emplace(name, span);
            if (!inserted.second) {
                return fail_parse("SYNQ-S004", span,
                                  "duplicate top-level declaration `" + name +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                  "rename the later declaration or reuse the existing binding according to future language semantics");
            }
            root->statements.push_back(new QubitDeclarationNode(name, qubit_count, line_number, span));
            continue;
        }

        if (operation == "fn" || operation == "kernel") {
            if (!active_features.is_enabled("callable-declarations")) {
                return fail_parse("SYNQ-P007", span, "callable declarations require an alpha feature opt-in",
                                  "add #[experimental(feature = \"callable-declarations\")] before the gated construct");
            }
            std::string name;
            std::vector<RoutineFormal> routine_formals;
            ParameterizedRoutineBody routine_body;
            if (operation == "kernel" && parse_parameterized_routine(argument, name, routine_formals, routine_body, span)) {
                if (!active_features.is_enabled("parameterized-quantum-routines") ||
                    !active_features.is_enabled("parameterized-quantum-gates")) {
                    return fail_parse("SYNQ-P007", span, "parameterized quantum routines require explicit Alpha opt-ins",
                                      "add parameterized-quantum-routines and parameterized-quantum-gates annotations before the routine");
                }
                const auto inserted = declared_names.emplace(name, span);
                if (!inserted.second) {
                    return fail_parse("SYNQ-S004", span, "duplicate top-level declaration `" + name +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                      "rename the later routine or reuse the existing declaration");
                }
                auto* routine = new CallableDeclarationNode(CallableDeclarationKind::Kernel, name, line_number, span);
                routine->formals = std::move(routine_formals);
                routine->parameterized_body = std::move(routine_body);
                root->statements.push_back(routine);
                continue;
            }
            std::string body_source;
            const bool has_body = operation == "kernel" && parse_bounded_kernel_body(argument, name, body_source);
            if (!has_body && !parse_callable_declaration(argument, name)) {
                if (operation == "kernel" && looks_like_parameterized_routine_declaration(argument)) {
                    return fail_parse("SYNQ-P017", span, "malformed parameterized quantum routine declaration",
                                      "use one documented typed signature with one formal-only supported gate body");
                }
                return fail_parse("SYNQ-P013", span, "malformed bounded callable declaration",
                                  "use fn <identifier>(), kernel <identifier>(), or kernel <identifier>() { quantum <gate> <register[index]> }");
            }
            const auto inserted = declared_names.emplace(name, span);
            if (!inserted.second) {
                return fail_parse("SYNQ-S004", span,
                                  "duplicate top-level declaration `" + name +
                                      "`; first declared on line " + std::to_string(inserted.first->second.line),
                                  "rename the callable or reuse the existing binding according to future language semantics");
            }
            const CallableDeclarationKind kind = operation == "fn" ? CallableDeclarationKind::Function
                                                                     : CallableDeclarationKind::Kernel;
            if (!has_body) {
                root->statements.push_back(new CallableDeclarationNode(kind, name, line_number, span));
                continue;
            }
            std::vector<std::string> quantum_arguments;
            if (!parse_quantum_arguments(trim(body_source.substr(std::string("quantum ").size())), quantum_arguments)) {
                return fail_parse("SYNQ-P013", span, "malformed bounded kernel body",
                                  "use exactly one supported quantum gate with explicit register operands");
            }
            QuantumGateNode* gate = make_quantum_gate_node(quantum_arguments, line_number, span);
            if (gate == nullptr) {
                return fail_parse("SYNQ-P013", span, "malformed bounded kernel body",
                                  "use exactly one supported quantum gate with explicit register operands");
            }
            const auto validation_error = synq::compiler::validate_quantum_gate_shape(*gate);
            if (validation_error.has_value()) {
                delete gate;
                return fail_parse("SYNQ-P013", span, "malformed bounded kernel body",
                                  "use exactly one supported quantum gate with explicit register operands");
            }
            if (gate->literal_angle.has_value() || uses_named_register_operand(*gate)) {
                delete gate;
                return fail_parse("SYNQ-P013", span, "bounded kernel bodies reject parameterized and named-register operands",
                                  "use one non-parameterized gate over the earlier default register until callable resource rules expand");
            }
            root->statements.push_back(new CallableDeclarationNode(kind, name, gate, line_number, span));
            continue;
        }

        if (operation == "call") {
            if (!active_features.is_enabled("callable-declarations")) {
                return fail_parse("SYNQ-P007", span, "callable calls require an alpha feature opt-in",
                                  "add #[experimental(feature = \"callable-declarations\")] before the gated construct");
            }
            std::string name;
            std::vector<std::string> actuals;
            if (parse_parameterized_routine_call(argument, name, actuals)) {
                if (!active_features.is_enabled("parameterized-quantum-routines")) {
                    return fail_parse("SYNQ-P007", span, "parameterized routine calls require an Alpha feature opt-in",
                                      "add #[experimental(feature = \"parameterized-quantum-routines\")] before the call");
                }
                root->statements.push_back(new CallableCallNode(name, std::move(actuals), line_number, span));
                continue;
            }
            if (!parse_callable_declaration(argument, name)) {
                if (looks_like_parameterized_routine_call(argument)) {
                    return fail_parse("SYNQ-P018", span, "malformed parameterized quantum routine call",
                                      "use call <earlier-routine>(<matching-literal-angle-and-or-register[index]-actuals>)");
                }
                return fail_parse("SYNQ-P013", span, "malformed bounded callable call",
                                  "use call <earlier-kernel-name>() with no arguments");
            }
            root->statements.push_back(new CallableCallNode(name, line_number, span));
            continue;
        }

        const bool known_instruction = operation == "print" || operation == "delay" ||
                                       operation == "quantum" || operation == "measure" || operation == "ai" ||
                                       operation == "if" || operation == "while" || operation == "qubit" ||
                                       operation == "fn" || operation == "kernel" || operation == "call";
        if (!known_instruction || argument.empty()) {
            return fail_parse("SYNQ-P003", span, "unsupported or incomplete recovery-profile instruction",
                              "use let, qubit, fn, kernel, call, print, delay, quantum, measure, ai, if, or while with the documented argument form");
        }
        if (operation == "delay" && !is_non_negative_integer(argument)) {
            return fail_parse("SYNQ-P004", span, "delay requires a non-negative integer number of milliseconds",
                              "use a non-negative whole number, such as delay 0");
        }

        if (operation == "if" || operation == "while") {
            if (!active_features.is_enabled("classical-control-flow")) {
                return fail_parse("SYNQ-P007", span, "classical control flow requires an alpha feature opt-in",
                                  "add #[experimental(feature = \"classical-control-flow\")] before the gated construct");
            }
            ClassicalCondition condition;
            std::string body_operation;
            std::string body_argument;
            const std::string connector = operation == "if" ? "then" : "do";
            if (!parse_control_flow_arguments(argument, connector, condition, span, body_operation, body_argument)) {
                return fail_parse("SYNQ-P009", span, "malformed bounded classical control-flow syntax",
                                  operation == "if" ? "use if true then quantum h q[0], if not ready then quantum h q[0], or if ready and enabled then quantum h q[0]" :
                                                      "use while false do measure q[0], while not ready do measure q[0], or while ready or fallback do measure q[0]");
            }
            synq::compiler::Diagnostic body_error;
            ASTNode* body = make_control_body_node(body_operation, body_argument, active_features,
                                                   line_number, span, body_error);
            if (body == nullptr) {
                synq::compiler::ParseResult result;
                result.diagnostics.push_back(std::move(body_error));
                return result;
            }
            root->statements.push_back(new ClassicalControlNode(
                operation == "if" ? ClassicalControlKind::If : ClassicalControlKind::While,
                condition, body, line_number, span));
        } else if (operation == "quantum") {
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
            if (uses_named_register_operand(*gate) &&
                !active_features.is_enabled("named-qubit-register-operands")) {
                delete gate;
                return fail_parse("SYNQ-P007", span, "named qubit register operands require an alpha feature opt-in",
                                  "add #[experimental(feature = \"named-qubit-register-operands\")] before the gated construct");
            }
            root->statements.push_back(gate);
        } else if (operation == "measure") {
            std::string register_name;
            std::size_t qubit_index = 0;
            std::optional<std::string> result_name;
            if (!parse_measurement_arguments(argument, register_name, qubit_index, result_name)) {
                return fail_parse("SYNQ-P008", span,
                                  "measurement requires one explicit qubit operand and an optional result identifier",
                                  "use measure q[index] or measure q[index] as <identifier>, for example measure q[0] as observed");
            }
            if (result_name.has_value()) {
                const auto inserted = declared_names.emplace(*result_name, span);
                if (!inserted.second) {
                    return fail_parse("SYNQ-S004", span,
                                      "duplicate top-level declaration `" + *result_name +
                                          "`; first declared on line " + std::to_string(inserted.first->second.line),
                                      "rename the measurement result or reuse the existing binding according to future language semantics");
                }
            }
            if (register_name != "q" && !active_features.is_enabled("named-qubit-register-operands")) {
                return fail_parse("SYNQ-P007", span, "named qubit register operands require an alpha feature opt-in",
                                  "add #[experimental(feature = \"named-qubit-register-operands\")] before the gated construct");
            }
            root->statements.push_back(new MeasurementNode(qubit_index, line_number, span,
                                                           std::move(result_name), std::move(register_name)));
        } else {
            root->statements.push_back(new InstructionNode(operation, {argument}, line_number, span));
        }
    }

    synq::compiler::ParseResult result;
    result.program = std::move(root);
    return result;
}

synq::compiler::ParseResult Parser::parseFileWithDiagnostics(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        return fail_parse("SYNQ-P001", {}, "could not open source file", "verify the file path and read permission");
    }
    return parseStreamWithDiagnostics(infile);
}

synq::compiler::ParseResult Parser::parseSourceWithDiagnostics(const std::string& source) {
    std::istringstream input(source);
    return parseStreamWithDiagnostics(input);
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
