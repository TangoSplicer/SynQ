// Bounded recovery-profile scoped name resolution implementation.
#include "name_resolution.h"

#include <algorithm>
#include <cctype>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace synq::compiler {

bool NameResolutionResult::ok() const {
    if (!program.has_value()) return false;
    for (const Diagnostic& diagnostic : diagnostics) {
        if (diagnostic.severity == DiagnosticSeverity::Error) return false;
    }
    return true;
}

const char* semantic_binding_kind_name(SemanticBindingKind kind) {
    switch (kind) {
        case SemanticBindingKind::Value: return "Value";
        case SemanticBindingKind::MutableCell: return "MutableCell";
        case SemanticBindingKind::MeasurementResult: return "MeasurementResult";
    }
    return "Value";
}

namespace {

bool is_identifier(const std::string& text) {
    if (text.empty()) return false;
    const auto first = static_cast<unsigned char>(text.front());
    if (!(std::isalpha(first) || text.front() == '_')) return false;

    for (char character : text) {
        const auto value = static_cast<unsigned char>(character);
        if (!(std::isalnum(value) || character == '_')) return false;
    }
    return true;
}

Diagnostic unresolved_binding_diagnostic(const HybridDeclaration& declaration) {
    return {
        "SYNQ-R001",
        DiagnosticSeverity::Error,
        declaration.span,
        "unknown or forward binding `" + declaration.source_value + "` in declaration initializer",
        "declare `" + declaration.source_value + "` on an earlier top-level line or use a supported literal"
    };
}

Diagnostic unresolved_control_condition_diagnostic(const ClassicalBooleanExpression& expression) {
    return {
        "SYNQ-R002",
        DiagnosticSeverity::Error,
        expression.span,
        "unknown or forward binding `" + expression.source_text + "` in classical control condition",
        "declare `" + expression.source_text + "` as an earlier Boolean binding or use true/false"
    };
}

Diagnostic non_boolean_control_condition_diagnostic(const ClassicalBooleanExpression& expression,
                                                    ClassicalStaticType actual_type) {
    return {
        "SYNQ-T001",
        DiagnosticSeverity::Error,
        expression.span,
        "classical control condition `" + expression.source_text + "` has static type " +
            classical_static_type_name(actual_type) + ", not Boolean",
        "use an earlier Boolean declaration or a true/false literal condition"
    };
}

Diagnostic invalid_boolean_expression_diagnostic(const ClassicalBooleanExpression& expression) {
    return {
        "SYNQ-T002",
        DiagnosticSeverity::Error,
        expression.span,
        "bounded classical Boolean expression has an invalid internal operator shape",
        "use one literal or identifier, not <atom>, or <atom> and/or <atom>"
    };
}

Diagnostic unresolved_integer_arithmetic_diagnostic(const ClassicalIntegerArithmeticExpression& expression) {
    return {
        "SYNQ-R003",
        DiagnosticSeverity::Error,
        expression.span,
        "unknown or forward binding `" + expression.source_text + "` in integer arithmetic initializer",
        "declare `" + expression.source_text + "` as an earlier Integer binding or use an Integer literal"
    };
}

Diagnostic non_integer_arithmetic_diagnostic(const ClassicalIntegerArithmeticExpression& expression,
                                             ClassicalStaticType actual_type) {
    return {
        "SYNQ-T003",
        DiagnosticSeverity::Error,
        expression.span,
        "integer arithmetic operand `" + expression.source_text + "` has static type " +
            classical_static_type_name(actual_type) + ", not Integer",
        "use an earlier Integer declaration or an Integer literal operand"
    };
}

Diagnostic invalid_integer_arithmetic_diagnostic(const ClassicalIntegerArithmeticExpression& expression) {
    return {
        "SYNQ-T004",
        DiagnosticSeverity::Error,
        expression.span,
        "bounded integer arithmetic expression has an invalid internal operator shape",
        "use exactly <atom> +, -, or * <atom> with Integer literal or identifier atoms"
    };
}

Diagnostic invalid_mutable_initializer_diagnostic(const SourceSpan& span, const std::string& name) {
    return {
        "SYNQ-S005",
        DiagnosticSeverity::Error,
        span,
        "mutable cell `" + name + "` requires an earlier supported Boolean, Integer, or String initializer",
        "use a supported literal or an earlier statically typed immutable/mutable binding"
    };
}

Diagnostic invalid_mutable_assignment_target_diagnostic(const SourceSpan& span, const std::string& name) {
    return {
        "SYNQ-S006",
        DiagnosticSeverity::Error,
        span,
        "assignment target `" + name + "` is not an earlier mutable cell",
        "declare `" + name + "` earlier with var and use whole-cell set syntax"
    };
}

Diagnostic invalid_measurement_feedback_result_diagnostic(const SourceSpan& span, const std::string& name) {
    return {
        "SYNQ-R007",
        DiagnosticSeverity::Error,
        span,
        "measurement result `" + name + "` is not an immediate single-use U4 feedback condition",
        "place `if " + name + " then quantum x register[index]` immediately after its named measurement and use it once"
    };
}

Diagnostic invalid_measurement_feedback_correction_diagnostic(const SourceSpan& span) {
    return {
        "SYNQ-R008",
        DiagnosticSeverity::Error,
        span,
        "U4 measurement feedback requires one direct conditional x correction over one explicit qubit operand",
        "use if <measurement-result> then quantum x register[index]"
    };
}

Diagnostic mutable_assignment_type_diagnostic(const SourceSpan& span, const std::string& name,
                                              ClassicalStaticType expected, ClassicalStaticType actual) {
    return {
        "SYNQ-S007",
        DiagnosticSeverity::Error,
        span,
        "assignment to mutable cell `" + name + "` has static type " +
            classical_static_type_name(actual) + ", not " + classical_static_type_name(expected),
        "assign an expression with the cell's existing static type"
    };
}

struct BindingInfo {
    std::size_t index = 0;
    ClassicalStaticType static_type = ClassicalStaticType::Unknown;
    std::string name;
    SemanticBindingKind kind = SemanticBindingKind::Value;
};

bool is_state_value_type(ClassicalStaticType type) {
    return type == ClassicalStaticType::Boolean || type == ClassicalStaticType::Integer ||
           type == ClassicalStaticType::String;
}

void append_binding_names(const std::unordered_map<std::string, BindingInfo>& bindings,
                          const std::vector<std::size_t>& binding_indices,
                          std::vector<std::string>& binding_names) {
    for (const std::size_t binding_index : binding_indices) {
        for (const auto& entry : bindings) {
            if (entry.second.index == binding_index) binding_names.push_back(entry.second.name);
        }
    }
}

bool resolve_boolean_expression(const ClassicalBooleanExpression& expression,
                                const std::unordered_map<std::string, BindingInfo>& bindings,
                                std::vector<std::size_t>& binding_indices,
                                Diagnostic& error) {
    if (expression.kind == ClassicalBooleanExpressionKind::BooleanLiteral) return true;
    if (expression.kind == ClassicalBooleanExpressionKind::IdentifierReference) {
        const auto binding = bindings.find(expression.source_text);
        if (binding == bindings.end()) {
            error = unresolved_control_condition_diagnostic(expression);
            return false;
        }
        if (binding->second.static_type != ClassicalStaticType::Boolean) {
            error = non_boolean_control_condition_diagnostic(expression, binding->second.static_type);
            return false;
        }
        binding_indices.push_back(binding->second.index);
        return true;
    }

    const std::size_t expected_operands = expression.kind == ClassicalBooleanExpressionKind::Not ? 1 : 2;
    if ((expression.kind != ClassicalBooleanExpressionKind::Not &&
         expression.kind != ClassicalBooleanExpressionKind::And &&
         expression.kind != ClassicalBooleanExpressionKind::Or) ||
        expression.operands.size() != expected_operands) {
        error = invalid_boolean_expression_diagnostic(expression);
        return false;
    }
    for (const ClassicalBooleanExpression& operand : expression.operands) {
        if (!resolve_boolean_expression(operand, bindings, binding_indices, error)) return false;
    }
    return true;
}

bool expression_references_identifier(const ClassicalBooleanExpression& expression, const std::string& name) {
    if (expression.kind == ClassicalBooleanExpressionKind::IdentifierReference && expression.source_text == name) {
        return true;
    }
    return std::any_of(expression.operands.begin(), expression.operands.end(), [&name](const auto& operand) {
        return expression_references_identifier(operand, name);
    });
}

bool is_direct_measurement_feedback_condition(const HybridControlFlow& control, const std::string& result_name) {
    return control.kind == ClassicalControlKind::If &&
           control.condition.kind == ClassicalConditionKind::IdentifierReference &&
           control.condition.expression.kind == ClassicalBooleanExpressionKind::IdentifierReference &&
           control.condition.expression.source_text == result_name &&
           control.condition.expression.operands.empty();
}

bool is_direct_measurement_feedback_correction(const HybridControlFlow& control) {
    const auto* gate = std::get_if<HybridQuantumGate>(&control.body);
    return gate != nullptr && gate->kind == QuantumGateKind::X && !gate->literal_angle.has_value() &&
           gate->qubit_indices.size() == 1 && gate->qubit_register_names.size() == 1;
}

bool resolve_integer_arithmetic_expression(const ClassicalIntegerArithmeticExpression& expression,
                                           const std::unordered_map<std::string, BindingInfo>& bindings,
                                           std::vector<std::size_t>& binding_indices,
                                           std::vector<std::string>& binding_names,
                                           Diagnostic& error) {
    if (expression.kind == ClassicalIntegerArithmeticExpressionKind::IntegerLiteral) return true;
    if (expression.kind == ClassicalIntegerArithmeticExpressionKind::IdentifierReference) {
        const auto binding = bindings.find(expression.source_text);
        if (binding == bindings.end()) {
            error = unresolved_integer_arithmetic_diagnostic(expression);
            return false;
        }
        if (binding->second.static_type != ClassicalStaticType::Integer) {
            error = non_integer_arithmetic_diagnostic(expression, binding->second.static_type);
            return false;
        }
        binding_indices.push_back(binding->second.index);
        binding_names.push_back(binding->second.name);
        return true;
    }
    if ((expression.kind != ClassicalIntegerArithmeticExpressionKind::Add &&
         expression.kind != ClassicalIntegerArithmeticExpressionKind::Subtract &&
         expression.kind != ClassicalIntegerArithmeticExpressionKind::Multiply) ||
        expression.operands.size() != 2) {
        error = invalid_integer_arithmetic_diagnostic(expression);
        return false;
    }
    for (const ClassicalIntegerArithmeticExpression& operand : expression.operands) {
        if (!resolve_integer_arithmetic_expression(operand, bindings, binding_indices, binding_names, error)) return false;
    }
    return true;
}

Diagnostic qubit_declaration_order_diagnostic(const SourceSpan& span, const std::string& register_name) {
    return {
        "SYNQ-Q001",
        DiagnosticSeverity::Error,
        span,
        register_name + "[index] is used before its explicit qubit declaration",
        "declare qubit " + register_name + "[positive-size] before quantum or measure statements using " +
            register_name + "[index]"
    };
}

Diagnostic qubit_index_range_diagnostic(const SourceSpan& span, const std::string& register_name,
                                        std::size_t index, std::size_t qubit_count) {
    return {
        "SYNQ-Q002",
        DiagnosticSeverity::Error,
        span,
        register_name + "[" + std::to_string(index) + "] is outside the declared register range " +
            register_name + "[0] through " + register_name + "[" +
            std::to_string(qubit_count - 1) + "]",
        "use an index smaller than the declared qubit " + register_name + "[" +
            std::to_string(qubit_count) + "] size"
    };
}

bool validate_qubit_operands(const std::vector<std::string>& register_names,
                             const std::vector<std::size_t>& indices,
                             const SourceSpan& span,
                             const std::unordered_map<std::string, std::size_t>& qubit_counts,
                             bool contains_explicit_default_register,
                             Diagnostic& error) {
    if (register_names.size() != indices.size()) {
        error = {"SYNQ-Q003", DiagnosticSeverity::Error, span,
                 "quantum operand register metadata does not match operand indices",
                 "use parser-produced bounded quantum operands"};
        return false;
    }
    for (std::size_t position = 0; position < indices.size(); ++position) {
        const std::string& register_name = register_names[position];
        const auto declaration = qubit_counts.find(register_name);
        if (declaration == qubit_counts.end()) {
            if (register_name == "q" && !contains_explicit_default_register) continue;
            error = qubit_declaration_order_diagnostic(span, register_name);
            return false;
        }
        if (indices[position] >= declaration->second) {
            error = qubit_index_range_diagnostic(span, register_name, indices[position], declaration->second);
            return false;
        }
    }
    return true;
}

bool validate_qubit_measurement(const HybridMeasurement& measurement,
                                const std::unordered_map<std::string, std::size_t>& qubit_counts,
                                bool contains_explicit_default_register,
                                Diagnostic& error) {
    return validate_qubit_operands({measurement.qubit_register_name}, {measurement.qubit_index}, measurement.span,
                                   qubit_counts, contains_explicit_default_register, error);
}

bool is_literal_angle_parameter(const std::string& value) {
    if (value == "pi" || value == "-pi") return true;
    std::size_t position = !value.empty() && value.front() == '-' ? 1 : 0;
    if (position != value.size()) {
        bool has_digit = false;
        bool has_decimal_point = false;
        bool decimal = true;
        for (; position < value.size(); ++position) {
            const unsigned char character = static_cast<unsigned char>(value[position]);
            if (std::isdigit(character) != 0) {
                has_digit = true;
            } else if (value[position] == '.' && !has_decimal_point) {
                has_decimal_point = true;
            } else {
                decimal = false;
                break;
            }
        }
        if (decimal && has_digit) return true;
    }
    const std::string prefix = value.rfind("-pi/", 0) == 0 ? "-pi/" : "pi/";
    const std::string denominator = value.rfind(prefix, 0) == 0 ? value.substr(prefix.size()) : "";
    return !denominator.empty() && denominator != "0" &&
           std::all_of(denominator.begin(), denominator.end(), [](unsigned char character) {
               return std::isdigit(character) != 0;
           });
}

bool parse_routine_actual_qubit(const std::string& source, std::string& register_name, std::size_t& index) {
    const std::size_t open = source.find('[');
    if (open == std::string::npos || open == 0 || source.back() != ']' ||
        source.find('[', open + 1) != std::string::npos || !is_identifier(source.substr(0, open))) {
        return false;
    }
    const std::string index_text = source.substr(open + 1, source.size() - open - 2);
    if (index_text.empty() || !std::all_of(index_text.begin(), index_text.end(), [](unsigned char character) {
            return std::isdigit(character) != 0;
        })) {
        return false;
    }
    index = 0;
    for (char character : index_text) {
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (index > (std::numeric_limits<std::size_t>::max() - digit) / 10) return false;
        index = index * 10 + digit;
    }
    register_name = source.substr(0, open);
    return true;
}

bool is_valid_parameterized_routine(const HybridCallableDeclaration& callable) {
    if (callable.kind != CallableDeclarationKind::Kernel || callable.formals.empty() ||
        !callable.parameterized_body.has_value()) {
        return false;
    }
    std::unordered_set<std::string> names;
    std::size_t angle_count = 0;
    for (const HybridRoutineFormal& formal : callable.formals) {
        if (!is_identifier(formal.name) || !names.emplace(formal.name).second) return false;
        if (formal.kind == RoutineFormalKind::Angle) ++angle_count;
    }
    if (angle_count > 1) return false;
    const HybridParameterizedRoutineBody& body = *callable.parameterized_body;
    const auto has_formal = [&callable](RoutineFormalKind kind, const std::string& name) {
        return std::any_of(callable.formals.begin(), callable.formals.end(), [&kind, &name](const HybridRoutineFormal& formal) {
            return formal.kind == kind && formal.name == name;
        });
    };
    const bool one_angle_one_qubit = callable.formals.size() == 2 &&
        callable.formals[0].kind == RoutineFormalKind::Angle && callable.formals[1].kind == RoutineFormalKind::Qubit &&
        body.angle_formal.has_value() && *body.angle_formal == callable.formals[0].name &&
        body.qubit_formals.size() == 1 && body.qubit_formals[0] == callable.formals[1].name &&
        (body.kind == QuantumGateKind::Rx || body.kind == QuantumGateKind::Ry ||
         body.kind == QuantumGateKind::Rz || body.kind == QuantumGateKind::Phase);
    const bool one_qubit = callable.formals.size() == 1 && callable.formals[0].kind == RoutineFormalKind::Qubit &&
        !body.angle_formal.has_value() && body.qubit_formals.size() == 1 &&
        body.qubit_formals[0] == callable.formals[0].name &&
        (body.kind == QuantumGateKind::H || body.kind == QuantumGateKind::X ||
         body.kind == QuantumGateKind::Y || body.kind == QuantumGateKind::Z);
    const bool two_qubits = callable.formals.size() == 2 && callable.formals[0].kind == RoutineFormalKind::Qubit &&
        callable.formals[1].kind == RoutineFormalKind::Qubit && !body.angle_formal.has_value() &&
        body.qubit_formals.size() == 2 && body.qubit_formals[0] == callable.formals[0].name &&
        body.qubit_formals[1] == callable.formals[1].name && body.qubit_formals[0] != body.qubit_formals[1] &&
        body.kind == QuantumGateKind::Cx;
    return one_angle_one_qubit || one_qubit || two_qubits;
}

Diagnostic invalid_parameterized_routine_diagnostic(const SourceSpan& span) {
    return {"SYNQ-R004", DiagnosticSeverity::Error, span,
            "parameterized quantum routine has an unsupported formal-to-body relation",
            "define one documented kernel signature with one formal-only supported gate body"};
}

Diagnostic parameterized_routine_call_diagnostic(const SourceSpan& span, const std::string& name) {
    return {"SYNQ-R004", DiagnosticSeverity::Error, span,
            "parameterized quantum routine call requires an earlier supported kernel `" + name + "`",
            "define the matching documented parameterized kernel before its call"};
}

Diagnostic parameterized_routine_call_shape_diagnostic(const SourceSpan& span) {
    return {"SYNQ-R005", DiagnosticSeverity::Error, span,
            "parameterized quantum routine call actuals do not match the declared ordered signature",
            "supply the exact documented literal-angle and/or register[index] actual sequence"};
}

}  // namespace

NameResolutionResult resolve_hybrid_names(const HybridProgram& program) {
    ResolvedHybridProgram resolved;
    resolved.nodes.reserve(program.nodes.size());
    std::unordered_map<std::string, BindingInfo> bindings;
    bool contains_explicit_default_register = false;
    for (const HybridNode& node : program.nodes) {
        const auto* qubits = std::get_if<HybridQubitDeclaration>(&node);
        if (qubits != nullptr && qubits->name == "q") {
            contains_explicit_default_register = true;
            break;
        }
    }
    std::unordered_map<std::string, std::size_t> qubit_counts;
    std::unordered_map<std::string, HybridCallableDeclaration> callable_definitions;
    std::unordered_set<std::string> consumed_measurement_results;
    bool terminal_feedback_seen = false;

    for (std::size_t node_index = 0; node_index < program.nodes.size(); ++node_index) {
        const HybridNode& node = program.nodes[node_index];
        if (terminal_feedback_seen) {
            NameResolutionResult result;
            result.diagnostics.push_back(invalid_measurement_feedback_result_diagnostic(
                {},
                "feedback result"));
            return result;
        }

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            std::optional<std::size_t> initializer_binding_index;
            std::vector<std::size_t> initializer_binding_indices;
            std::vector<std::string> initializer_binding_names;
            ClassicalStaticType initializer_static_type = declaration->initializer.static_type;
            if (declaration->initializer.kind == ClassicalExpressionKind::IdentifierReference &&
                is_identifier(declaration->source_value)) {
                const auto binding = bindings.find(declaration->source_value);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(unresolved_binding_diagnostic(*declaration));
                    return result;
                }
                if (binding->second.kind == SemanticBindingKind::MutableCell) {
                    NameResolutionResult result;
                    result.diagnostics.push_back({"SYNQ-S005", DiagnosticSeverity::Error, declaration->span,
                                                  "immutable declaration `" + declaration->name +
                                                      "` cannot read mutable cell `" + declaration->source_value + "`",
                                                  "use var for a time-dependent value or keep let initializers immutable"});
                    return result;
                }
                initializer_binding_index = binding->second.index;
                initializer_static_type = binding->second.static_type;
                initializer_binding_names.push_back(binding->second.name);
            }
            if (declaration->initializer.kind == ClassicalExpressionKind::IntegerArithmeticExpression) {
                Diagnostic arithmetic_error;
                if (!declaration->initializer.integer_arithmetic.has_value() ||
                    !resolve_integer_arithmetic_expression(*declaration->initializer.integer_arithmetic, bindings,
                                                           initializer_binding_indices, initializer_binding_names,
                                                           arithmetic_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(arithmetic_error));
                    return result;
                }
                initializer_static_type = ClassicalStaticType::Integer;
            }
            if (declaration->initializer.kind == ClassicalExpressionKind::BooleanExpression) {
                Diagnostic boolean_error;
                if (!declaration->initializer.boolean_expression.has_value() ||
                    !resolve_boolean_expression(*declaration->initializer.boolean_expression, bindings,
                                                initializer_binding_indices, boolean_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(boolean_error));
                    return result;
                }
                initializer_static_type = ClassicalStaticType::Boolean;
                append_binding_names(bindings, initializer_binding_indices, initializer_binding_names);
            }

            resolved.nodes.emplace_back(ResolvedHybridDeclaration{*declaration, initializer_binding_index,
                                                                   initializer_static_type,
                                                                   std::move(initializer_binding_indices)});
            resolved.semantic_bindings.push_back({declaration->name, SemanticBindingKind::Value,
                                                  initializer_static_type, node_index, declaration->span,
                                                  std::move(initializer_binding_names)});
            bindings.emplace(declaration->name, BindingInfo{node_index, initializer_static_type,
                                                            declaration->name, SemanticBindingKind::Value});
            continue;
        }

        if (const auto* declaration = std::get_if<HybridMutableDeclaration>(&node)) {
            std::optional<std::size_t> initializer_binding_index;
            std::vector<std::size_t> initializer_binding_indices;
            std::vector<std::string> initializer_binding_names;
            ClassicalStaticType initializer_static_type = declaration->initializer.static_type;
            if (declaration->initializer.kind == ClassicalExpressionKind::IdentifierReference &&
                is_identifier(declaration->source_value)) {
                const auto binding = bindings.find(declaration->source_value);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(invalid_mutable_initializer_diagnostic(declaration->span, declaration->name));
                    return result;
                }
                initializer_binding_index = binding->second.index;
                initializer_static_type = binding->second.static_type;
                initializer_binding_names.push_back(binding->second.name);
            }
            if (declaration->initializer.kind == ClassicalExpressionKind::IntegerArithmeticExpression) {
                Diagnostic arithmetic_error;
                if (!declaration->initializer.integer_arithmetic.has_value() ||
                    !resolve_integer_arithmetic_expression(*declaration->initializer.integer_arithmetic, bindings,
                                                           initializer_binding_indices, initializer_binding_names,
                                                           arithmetic_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(arithmetic_error));
                    return result;
                }
                initializer_static_type = ClassicalStaticType::Integer;
            }
            if (declaration->initializer.kind == ClassicalExpressionKind::BooleanExpression) {
                Diagnostic boolean_error;
                if (!declaration->initializer.boolean_expression.has_value() ||
                    !resolve_boolean_expression(*declaration->initializer.boolean_expression, bindings,
                                                initializer_binding_indices, boolean_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(boolean_error));
                    return result;
                }
                initializer_static_type = ClassicalStaticType::Boolean;
                append_binding_names(bindings, initializer_binding_indices, initializer_binding_names);
            }
            if (!is_state_value_type(initializer_static_type)) {
                NameResolutionResult result;
                result.diagnostics.push_back(invalid_mutable_initializer_diagnostic(declaration->span, declaration->name));
                return result;
            }
            resolved.nodes.emplace_back(ResolvedHybridMutableDeclaration{*declaration, initializer_binding_index,
                                                                          initializer_static_type,
                                                                          std::move(initializer_binding_indices)});
            resolved.semantic_bindings.push_back({declaration->name, SemanticBindingKind::MutableCell,
                                                  initializer_static_type, node_index, declaration->span,
                                                  std::move(initializer_binding_names)});
            bindings.emplace(declaration->name, BindingInfo{node_index, initializer_static_type,
                                                            declaration->name, SemanticBindingKind::MutableCell});
            continue;
        }

        if (const auto* assignment = std::get_if<HybridAssignment>(&node)) {
            const auto target = bindings.find(assignment->target_name);
            if (target == bindings.end() || target->second.kind != SemanticBindingKind::MutableCell) {
                NameResolutionResult result;
                result.diagnostics.push_back(invalid_mutable_assignment_target_diagnostic(assignment->span,
                                                                                            assignment->target_name));
                return result;
            }
            std::optional<std::size_t> value_binding_index;
            std::vector<std::size_t> value_binding_indices;
            std::vector<std::string> ignored_binding_names;
            ClassicalStaticType value_static_type = assignment->value.static_type;
            if (assignment->value.kind == ClassicalExpressionKind::IdentifierReference &&
                is_identifier(assignment->source_value)) {
                const auto binding = bindings.find(assignment->source_value);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(mutable_assignment_type_diagnostic(
                        assignment->span, assignment->target_name, target->second.static_type,
                        ClassicalStaticType::Unknown));
                    return result;
                }
                value_binding_index = binding->second.index;
                value_static_type = binding->second.static_type;
            }
            if (assignment->value.kind == ClassicalExpressionKind::IntegerArithmeticExpression) {
                Diagnostic arithmetic_error;
                if (!assignment->value.integer_arithmetic.has_value() ||
                    !resolve_integer_arithmetic_expression(*assignment->value.integer_arithmetic, bindings,
                                                           value_binding_indices, ignored_binding_names,
                                                           arithmetic_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(arithmetic_error));
                    return result;
                }
                value_static_type = ClassicalStaticType::Integer;
            }
            if (assignment->value.kind == ClassicalExpressionKind::BooleanExpression) {
                Diagnostic boolean_error;
                if (!assignment->value.boolean_expression.has_value() ||
                    !resolve_boolean_expression(*assignment->value.boolean_expression, bindings,
                                                value_binding_indices, boolean_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(boolean_error));
                    return result;
                }
                value_static_type = ClassicalStaticType::Boolean;
            }
            if (!is_state_value_type(value_static_type) || value_static_type != target->second.static_type) {
                NameResolutionResult result;
                result.diagnostics.push_back(mutable_assignment_type_diagnostic(
                    assignment->span, assignment->target_name, target->second.static_type, value_static_type));
                return result;
            }
            resolved.nodes.emplace_back(ResolvedHybridAssignment{*assignment, target->second.index,
                                                                  target->second.static_type, value_binding_index,
                                                                  value_static_type,
                                                                  std::move(value_binding_indices)});
            continue;
        }

        if (const auto* qubits = std::get_if<HybridQubitDeclaration>(&node)) {
            qubit_counts.emplace(qubits->name, qubits->qubit_count);
            resolved.nodes.emplace_back(*qubits);
            continue;
        }

        if (const auto* callable = std::get_if<HybridCallableDeclaration>(&node)) {
            if (!callable->formals.empty() || callable->parameterized_body.has_value()) {
                if (!is_valid_parameterized_routine(*callable)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(invalid_parameterized_routine_diagnostic(callable->span));
                    return result;
                }
                callable_definitions.emplace(callable->name, *callable);
                resolved.nodes.emplace_back(*callable);
                continue;
            }
            if (callable->body.has_value()) {
                Diagnostic qubit_error;
                if (!validate_qubit_operands(callable->body->qubit_register_names,
                                             callable->body->qubit_indices, callable->body->span,
                                             qubit_counts, contains_explicit_default_register, qubit_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(qubit_error));
                    return result;
                }
            }
            callable_definitions.emplace(callable->name, *callable);
            resolved.nodes.emplace_back(*callable);
            continue;
        }

        if (const auto* call = std::get_if<HybridCallableCall>(&node)) {
            const auto target = callable_definitions.find(call->name);
            if (!call->arguments.empty()) {
                if (target == callable_definitions.end() || !is_valid_parameterized_routine(target->second)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(parameterized_routine_call_diagnostic(call->span, call->name));
                    return result;
                }
                const HybridCallableDeclaration& routine = target->second;
                if (call->arguments.size() != routine.formals.size()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(parameterized_routine_call_shape_diagnostic(call->span));
                    return result;
                }
                std::vector<std::string> register_names;
                std::vector<std::size_t> qubit_indices;
                for (std::size_t position = 0; position < routine.formals.size(); ++position) {
                    const HybridRoutineFormal& formal = routine.formals[position];
                    const std::string& actual = call->arguments[position];
                    if (formal.kind == RoutineFormalKind::Angle) {
                        if (!is_literal_angle_parameter(actual)) {
                            NameResolutionResult result;
                            result.diagnostics.push_back(parameterized_routine_call_shape_diagnostic(call->span));
                            return result;
                        }
                        continue;
                    }
                    std::string register_name;
                    std::size_t qubit_index = 0;
                    if (!parse_routine_actual_qubit(actual, register_name, qubit_index)) {
                        NameResolutionResult result;
                        result.diagnostics.push_back(parameterized_routine_call_shape_diagnostic(call->span));
                        return result;
                    }
                    register_names.push_back(std::move(register_name));
                    qubit_indices.push_back(qubit_index);
                }
                Diagnostic qubit_error;
                if (!validate_qubit_operands(register_names, qubit_indices, call->span, qubit_counts,
                                             contains_explicit_default_register, qubit_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(qubit_error));
                    return result;
                }
                if (qubit_indices.size() == 2 && register_names[0] == register_names[1] &&
                    qubit_indices[0] == qubit_indices[1]) {
                    NameResolutionResult result;
                    result.diagnostics.push_back({"SYNQ-R006", DiagnosticSeverity::Error, call->span,
                                                  "two-qubit parameterized routine call aliases one physical source qubit",
                                                  "use two distinct declared register[index] operands"});
                    return result;
                }
                resolved.nodes.emplace_back(*call);
                continue;
            }
            if (target != callable_definitions.end() && target->second.parameterized_body.has_value()) {
                NameResolutionResult result;
                result.diagnostics.push_back(parameterized_routine_call_shape_diagnostic(call->span));
                return result;
            }
            if (target == callable_definitions.end() || !target->second.body.has_value() ||
                target->second.kind != CallableDeclarationKind::Kernel) {
                NameResolutionResult result;
                result.diagnostics.push_back({"SYNQ-R003", DiagnosticSeverity::Error, call->span,
                                              "bounded callable call requires an earlier one-gate kernel definition",
                                              "declare kernel <name>() { quantum <gate> q[index] } before call <name>()"});
                return result;
            }
            resolved.nodes.emplace_back(*call);
            continue;
        }

        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            Diagnostic qubit_error;
            if (!validate_qubit_operands(gate->qubit_register_names, gate->qubit_indices, gate->span, qubit_counts,
                                         contains_explicit_default_register, qubit_error)) {
                NameResolutionResult result;
                result.diagnostics.push_back(std::move(qubit_error));
                return result;
            }
            resolved.nodes.emplace_back(*gate);
            continue;
        }

        if (const auto* control = std::get_if<HybridControlFlow>(&node)) {
            if (control->feedback_enabled && control->condition.kind == ClassicalConditionKind::IdentifierReference) {
                const auto binding = bindings.find(control->condition.source_text);
                if (binding != bindings.end() && binding->second.kind == SemanticBindingKind::MeasurementResult &&
                    consumed_measurement_results.find(control->condition.source_text) != consumed_measurement_results.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(invalid_measurement_feedback_result_diagnostic(
                        control->span, control->condition.source_text));
                    return result;
                }
                if (binding != bindings.end() && binding->second.kind == SemanticBindingKind::MeasurementResult) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(invalid_measurement_feedback_result_diagnostic(
                        control->span, control->condition.source_text));
                    return result;
                }
            }
            std::optional<std::size_t> condition_binding_index;
            std::vector<std::size_t> condition_binding_indices;
            Diagnostic condition_error;
            if (!resolve_boolean_expression(control->condition.expression, bindings, condition_binding_indices,
                                            condition_error)) {
                NameResolutionResult result;
                result.diagnostics.push_back(std::move(condition_error));
                return result;
            }
            Diagnostic qubit_error;
            bool qubits_valid = false;
            if (const auto* gate = std::get_if<HybridQuantumGate>(&control->body)) {
                qubits_valid = validate_qubit_operands(gate->qubit_register_names, gate->qubit_indices, gate->span,
                                                        qubit_counts, contains_explicit_default_register, qubit_error);
            } else {
                const auto& measurement = std::get<HybridMeasurement>(control->body);
                qubits_valid = validate_qubit_measurement(measurement, qubit_counts,
                                                           contains_explicit_default_register, qubit_error);
            }
            if (!qubits_valid) {
                NameResolutionResult result;
                result.diagnostics.push_back(std::move(qubit_error));
                return result;
            }
            if (control->condition.kind == ClassicalConditionKind::IdentifierReference &&
                condition_binding_indices.size() == 1) {
                condition_binding_index = condition_binding_indices.front();
            }
            resolved.nodes.emplace_back(ResolvedHybridControlFlow{*control, condition_binding_index,
                                                                    std::move(condition_binding_indices)});
            continue;
        }

        const auto& measurement = std::get<HybridMeasurement>(node);
        Diagnostic qubit_error;
        if (!validate_qubit_measurement(measurement, qubit_counts, contains_explicit_default_register, qubit_error)) {
            NameResolutionResult result;
            result.diagnostics.push_back(std::move(qubit_error));
            return result;
        }
        if (measurement.result_name.has_value()) {
            const std::string& result_name = *measurement.result_name;
            resolved.semantic_bindings.push_back({*measurement.result_name, SemanticBindingKind::MeasurementResult,
                                                  ClassicalStaticType::Boolean, node_index, measurement.span, {}});
            bindings.emplace(*measurement.result_name, BindingInfo{node_index, ClassicalStaticType::Boolean,
                                                                    *measurement.result_name,
                                                                    SemanticBindingKind::MeasurementResult});

            if (measurement.feedback_enabled && node_index + 1 < program.nodes.size()) {
                const auto* next_control = std::get_if<HybridControlFlow>(&program.nodes[node_index + 1]);
                if (next_control != nullptr && next_control->feedback_enabled &&
                    expression_references_identifier(next_control->condition.expression, result_name)) {
                    if (!is_direct_measurement_feedback_condition(*next_control, result_name)) {
                        NameResolutionResult result;
                        result.diagnostics.push_back(invalid_measurement_feedback_result_diagnostic(
                            next_control->span, result_name));
                        return result;
                    }
                    if (!is_direct_measurement_feedback_correction(*next_control)) {
                        NameResolutionResult result;
                        result.diagnostics.push_back(invalid_measurement_feedback_correction_diagnostic(
                            next_control->span));
                        return result;
                    }
                    const auto& correction = std::get<HybridQuantumGate>(next_control->body);
                    if (!validate_qubit_operands(correction.qubit_register_names, correction.qubit_indices,
                                                 correction.span, qubit_counts,
                                                 contains_explicit_default_register, qubit_error)) {
                        NameResolutionResult result;
                        result.diagnostics.push_back(std::move(qubit_error));
                        return result;
                    }
                    resolved.nodes.emplace_back(ResolvedHybridMeasurementFeedback{
                        measurement, *next_control, node_index});
                    consumed_measurement_results.insert(result_name);
                    terminal_feedback_seen = true;
                    ++node_index;
                    continue;
                }
            }
        }
        resolved.nodes.emplace_back(measurement);
    }

    NameResolutionResult result;
    result.program = std::move(resolved);
    return result;
}

std::string render_semantic_environment(const ResolvedHybridProgram& program) {
    std::ostringstream output;
    output << "semantic environment: top-level classical bindings\n";
    if (program.semantic_bindings.empty()) {
        output << "(no classical bindings)\n";
        return output.str();
    }
    for (const SemanticBinding& binding : program.semantic_bindings) {
        output << "binding " << binding.name << " | " << semantic_binding_kind_name(binding.kind)
               << " | " << classical_static_type_name(binding.static_type)
               << " | line " << binding.span.line;
        if (!binding.dependency_names.empty()) {
            output << " | depends-on ";
            for (std::size_t index = 0; index < binding.dependency_names.size(); ++index) {
                if (index != 0) output << ", ";
                output << binding.dependency_names[index];
            }
        }
        output << "\n";
    }
    return output.str();
}

}  // namespace synq::compiler
