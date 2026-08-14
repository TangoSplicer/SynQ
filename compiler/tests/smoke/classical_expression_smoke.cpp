// Bounded non-evaluating classical-expression/type and condition-reference smoke coverage.
#include <cstdlib>
#include <iostream>
#include <variant>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "classical-expression smoke failure: " << message << '\n';
        std::exit(1);
    }
}

}  // namespace

int main() {
    Parser parser;
    auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let inherited = ready\n"
        "let count = 1\n"
        "let formula = count + 1\n"
        "if inherited then quantum h q[0]\n"
        "while ready do measure q[0]\n");
    expect(parsed.ok(), "identifier conditions and declarations should parse inside the bounded profile");

    const auto* if_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[4]);
    expect(if_node != nullptr && if_node->condition.kind == ClassicalConditionKind::IdentifierReference,
           "control condition should preserve an identifier-reference form");
    expect(if_node->condition.source_text == "inherited", "condition should preserve its source text");

    auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    expect(lowered.ok(), "typed declarations and identifier conditions should lower to Hybrid IR");
    const auto* ready = std::get_if<synq::compiler::HybridDeclaration>(&lowered.program->nodes[0]);
    const auto* inherited = std::get_if<synq::compiler::HybridDeclaration>(&lowered.program->nodes[1]);
    const auto* formula = std::get_if<synq::compiler::HybridDeclaration>(&lowered.program->nodes[3]);
    expect(ready != nullptr && ready->initializer.kind == synq::compiler::ClassicalExpressionKind::BooleanLiteral &&
               ready->initializer.static_type == synq::compiler::ClassicalStaticType::Boolean,
           "boolean literal should have a bounded Boolean static type");
    expect(inherited != nullptr && inherited->initializer.kind == synq::compiler::ClassicalExpressionKind::IdentifierReference &&
               inherited->initializer.static_type == synq::compiler::ClassicalStaticType::Unknown,
           "identifier initializer should remain an unresolved typed reference before resolution");
    expect(formula != nullptr && formula->initializer.kind == synq::compiler::ClassicalExpressionKind::OpaqueSource,
           "expression-like source should remain opaque and unevaluated");

    auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    expect(resolved.ok(), "earlier Boolean declarations should resolve condition references");
    const auto* resolved_inherited = std::get_if<synq::compiler::ResolvedHybridDeclaration>(&resolved.program->nodes[1]);
    const auto* resolved_if = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes[4]);
    const auto* resolved_while = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes[5]);
    expect(resolved_inherited != nullptr && resolved_inherited->initializer_binding_index == 0 &&
               resolved_inherited->initializer_static_type == synq::compiler::ClassicalStaticType::Boolean,
           "identifier declaration should inherit the earlier Boolean static type without evaluation");
    expect(resolved_if != nullptr && resolved_if->condition_binding_index == 1,
           "if condition should resolve the earlier Boolean alias");
    expect(resolved_while != nullptr && resolved_while->condition_binding_index == 0,
           "while condition should resolve the direct Boolean binding");

    auto unknown = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "if missing then quantum h q[0]\n");
    expect(unknown.ok(), "identifier condition syntax should remain a resolver concern");
    auto unknown_hir = synq::compiler::lower_to_hybrid_ir(*unknown.program);
    auto unknown_resolved = synq::compiler::resolve_hybrid_names(*unknown_hir.program);
    expect(!unknown_resolved.ok() && unknown_resolved.diagnostics[0].code == "SYNQ-R002",
           "unknown condition binding should have a dedicated resolution diagnostic");

    auto non_boolean = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let count = 1\n"
        "if count then quantum h q[0]\n");
    expect(non_boolean.ok(), "non-Boolean condition syntax should parse before static type validation");
    auto non_boolean_hir = synq::compiler::lower_to_hybrid_ir(*non_boolean.program);
    auto non_boolean_resolved = synq::compiler::resolve_hybrid_names(*non_boolean_hir.program);
    expect(!non_boolean_resolved.ok() && non_boolean_resolved.diagnostics[0].code == "SYNQ-T001",
           "non-Boolean condition binding should have a dedicated static-type diagnostic");

    return 0;
}
