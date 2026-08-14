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

}  // namespace

NameResolutionResult resolve_hybrid_names(const HybridProgram& program) {
    ResolvedHybridProgram resolved;
    resolved.nodes.reserve(program.nodes.size());
    std::unordered_map<std::string, std::size_t> bindings;

    for (std::size_t node_index = 0; node_index < program.nodes.size(); ++node_index) {
        const HybridNode& node = program.nodes[node_index];

        if (const auto* declaration = std::get_if<HybridDeclaration>(&node)) {
            std::optional<std::size_t> initializer_binding_index;
            if (declaration->literal_kind == ClassicalLiteralKind::SourceText &&
                is_identifier(declaration->source_value)) {
                const auto binding = bindings.find(declaration->source_value);
                if (binding == bindings.end()) {
                    NameResolutionResult result;
                    result.diagnostics.push_back(unresolved_binding_diagnostic(*declaration));
                    return result;
                }
                initializer_binding_index = binding->second;
            }

            resolved.nodes.emplace_back(ResolvedHybridDeclaration{*declaration, initializer_binding_index});
            bindings.emplace(declaration->name, node_index);
            continue;
        }

        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            resolved.nodes.emplace_back(*gate);
            continue;
        }

        resolved.nodes.emplace_back(std::get<HybridMeasurement>(node));
    }

    NameResolutionResult result;
    result.program = std::move(resolved);
    return result;
}

}  // namespace synq::compiler
