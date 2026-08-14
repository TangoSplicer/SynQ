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

Diagnostic unresolved_control_condition_diagnostic(const HybridControlFlow& control) {
    return {
        "SYNQ-R002",
        DiagnosticSeverity::Error,
        control.condition.span,
        "unknown or forward binding `" + control.condition.source_text + "` in classical control condition",
        "declare `" + control.condition.source_text + "` as an earlier Boolean binding or use true/false"
    };
}

Diagnostic non_boolean_control_condition_diagnostic(const HybridControlFlow& control,
                                                    ClassicalStaticType actual_type) {
    return {
        "SYNQ-T001",
        DiagnosticSeverity::Error,
        control.condition.span,
        "classical control condition `" + control.condition.source_text + "` has static type " +
            classical_static_type_name(actual_type) + ", not Boolean",
        "use an earlier Boolean declaration or a true/false literal condition"
    };
}

struct BindingInfo {
    std::size_t index = 0;
    ClassicalStaticType static_type = ClassicalStaticType::Unknown;
};

}  // namespace

NameResolutionResult resolve_hybrid_names(const HybridProgram& program) {
    ResolvedHybridProgram resolved;
    resolved.nodes.reserve(program.nodes.size());
    std::unordered_map<std::string, BindingInfo> bindings;

    for (std::size_t node_index = 0; node_index < program.nodes.size(); ++node_index) {
        const HybridNode& node = program.nodes[node_index];

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            std::optional<std::size_t> initializer_binding_index;
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

            resolved.nodes.emplace_back(ResolvedHybridDeclaration{*declaration, initializer_binding_index,
                                                                   initializer_static_type});
            bindings.emplace(declaration->name, BindingInfo{node_index, initializer_static_type});
            continue;
        }

        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            resolved.nodes.emplace_back(*gate);
            continue;
        }

        if (const auto* control = std::get_if<HybridControlFlow>(&node)) {
            std::optional<std::size_t> condition_binding_index;
            if (control->condition.kind == ClassicalConditionKind::IdentifierReference) {
                const auto binding = bindings.find(control->condition.source_text);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(unresolved_control_condition_diagnostic(*control));
                    return result;
                }
                if (binding->second.static_type != ClassicalStaticType::Boolean) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(non_boolean_control_condition_diagnostic(*control,
                                                                                           binding->second.static_type));
                    return result;
                }
                condition_binding_index = binding->second.index;
            }
            resolved.nodes.emplace_back(ResolvedHybridControlFlow{*control, condition_binding_index});
            continue;
        }

        resolved.nodes.emplace_back(std::get<HybridMeasurement>(node));
    }

    NameResolutionResult result;
    result.program = std::move(resolved);
    return result;
}

}  // namespace synq::compiler
