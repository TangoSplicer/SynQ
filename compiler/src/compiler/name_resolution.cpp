// Bounded recovery-profile scoped name resolution implementation.
#include "name_resolution.h"

#include <cctype>
#include <string>
#include <unordered_map>
#include <utility>

namespace synq::compiler {

bool NameResolutionResult::ok() const {
    if (!program.has_value()) return false;
    for (const Diagnostic& diagnostic : diagnostics) {
        if (diagnostic.severity == DiagnosticSeverity::Error) return false;
    }
    return true;
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

struct BindingInfo {
    std::size_t index = 0;
    ClassicalStaticType static_type = ClassicalStaticType::Unknown;
};

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

bool resolve_integer_arithmetic_expression(const ClassicalIntegerArithmeticExpression& expression,
                                           const std::unordered_map<std::string, BindingInfo>& bindings,
                                           std::vector<std::size_t>& binding_indices,
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
        if (!resolve_integer_arithmetic_expression(operand, bindings, binding_indices, error)) return false;
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

    for (std::size_t node_index = 0; node_index < program.nodes.size(); ++node_index) {
        const HybridNode& node = program.nodes[node_index];

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            std::optional<std::size_t> initializer_binding_index;
            std::vector<std::size_t> initializer_binding_indices;
            ClassicalStaticType initializer_static_type = declaration->initializer.static_type;
            if (declaration->initializer.kind == ClassicalExpressionKind::IdentifierReference &&
                is_identifier(declaration->source_value)) {
                const auto binding = bindings.find(declaration->source_value);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(unresolved_binding_diagnostic(*declaration));
                    return result;
                }
                initializer_binding_index = binding->second.index;
                initializer_static_type = binding->second.static_type;
            }
            if (declaration->initializer.kind == ClassicalExpressionKind::IntegerArithmeticExpression) {
                Diagnostic arithmetic_error;
                if (!declaration->initializer.integer_arithmetic.has_value() ||
                    !resolve_integer_arithmetic_expression(*declaration->initializer.integer_arithmetic, bindings,
                                                           initializer_binding_indices, arithmetic_error)) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(std::move(arithmetic_error));
                    return result;
                }
                initializer_static_type = ClassicalStaticType::Integer;
            }

            resolved.nodes.emplace_back(ResolvedHybridDeclaration{*declaration, initializer_binding_index,
                                                                   initializer_static_type,
                                                                   std::move(initializer_binding_indices)});
            bindings.emplace(declaration->name, BindingInfo{node_index, initializer_static_type});
            continue;
        }

        if (const auto* qubits = std::get_if<HybridQubitDeclaration>(&node)) {
            qubit_counts.emplace(qubits->name, qubits->qubit_count);
            resolved.nodes.emplace_back(*qubits);
            continue;
        }

        if (const auto* callable = std::get_if<HybridCallableDeclaration>(&node)) {
            resolved.nodes.emplace_back(*callable);
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
        resolved.nodes.emplace_back(measurement);
        if (measurement.result_name.has_value()) {
            bindings.emplace(*measurement.result_name, BindingInfo{node_index, ClassicalStaticType::Boolean});
        }
    }

    NameResolutionResult result;
    result.program = std::move(resolved);
    return result;
}

}  // namespace synq::compiler
