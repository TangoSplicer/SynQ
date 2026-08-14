#include <cstdlib>
#include <iostream>
#include <variant>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "boolean-expression smoke failure: " << message << '\n';
        std::exit(1);
    }
}

}  // namespace

int main() {
    Parser parser;
    auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let fallback = false\n"
        "if not ready then quantum h q[0]\n"
        "while ready and fallback do measure q[0]\n"
        "if ready or fallback then quantum x q[0]\n");
    expect(parsed.ok(), "bounded Boolean operator forms should parse after the alpha opt-in");

    const auto* not_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[2]);
    const auto* and_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[3]);
    const auto* or_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[4]);
    expect(not_node != nullptr && not_node->condition.kind == ClassicalConditionKind::BooleanExpression &&
               not_node->condition.expression.kind == ClassicalBooleanExpressionKind::Not &&
               not_node->condition.expression.operands.size() == 1,
           "not should build a typed unary Boolean expression");
    expect(and_node != nullptr && and_node->condition.expression.kind == ClassicalBooleanExpressionKind::And &&
               and_node->condition.expression.operands.size() == 2,
           "and should build a typed binary Boolean expression");
    expect(or_node != nullptr && or_node->condition.expression.kind == ClassicalBooleanExpressionKind::Or &&
               or_node->condition.expression.operands.size() == 2,
           "or should build a typed binary Boolean expression");

    auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    expect(lowered.ok(), "typed Boolean expressions should lower into Hybrid IR without evaluation");
    auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    expect(resolved.ok(), "earlier Boolean bindings should resolve every Boolean expression leaf");
    const auto* resolved_not = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes[2]);
    const auto* resolved_and = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes[3]);
    const auto* resolved_or = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes[4]);
    expect(resolved_not != nullptr && resolved_not->condition_binding_indices.size() == 1 &&
               resolved_not->condition_binding_indices[0] == 0,
           "not should preserve its resolved Boolean leaf binding");
    expect(resolved_and != nullptr && resolved_and->condition_binding_indices.size() == 2 &&
               resolved_and->condition_binding_indices[0] == 0 && resolved_and->condition_binding_indices[1] == 1,
           "and should preserve both resolved Boolean leaf bindings in source order");
    expect(resolved_or != nullptr && resolved_or->condition_binding_indices.size() == 2,
           "or should preserve both resolved Boolean leaf bindings");

    auto unknown = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "if ready and missing then quantum h q[0]\n");
    expect(unknown.ok(), "unknown Boolean expression leaves should be resolver concerns");
    auto unknown_hir = synq::compiler::lower_to_hybrid_ir(*unknown.program);
    auto unknown_resolved = synq::compiler::resolve_hybrid_names(*unknown_hir.program);
    expect(!unknown_resolved.ok() && unknown_resolved.diagnostics[0].code == "SYNQ-R002",
           "unknown Boolean expression leaves should use the dedicated resolver diagnostic");

    auto non_boolean = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let count = 1\n"
        "if ready and count then quantum h q[0]\n");
    expect(non_boolean.ok(), "non-Boolean expression leaves should parse before static-type validation");
    auto non_boolean_hir = synq::compiler::lower_to_hybrid_ir(*non_boolean.program);
    auto non_boolean_resolved = synq::compiler::resolve_hybrid_names(*non_boolean_hir.program);
    expect(!non_boolean_resolved.ok() && non_boolean_resolved.diagnostics[0].code == "SYNQ-T001",
           "non-Boolean expression leaves should use the static-type diagnostic");

    auto malformed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "if not ready and fallback then quantum h q[0]\n");
    expect(!malformed.ok() && malformed.diagnostics[0].code == "SYNQ-P009",
           "combined operators remain outside the bounded no-precedence grammar");

    synq::compiler::HybridProgram invalid_tree_program;
    ClassicalCondition invalid_condition;
    invalid_condition.kind = ClassicalConditionKind::BooleanExpression;
    invalid_condition.source_text = "ready and fallback";
    invalid_condition.expression = {ClassicalBooleanExpressionKind::And, false, invalid_condition.source_text,
                                    {1, 1, 20}, {}};
    invalid_tree_program.nodes.emplace_back(synq::compiler::HybridControlFlow{
        ClassicalControlKind::If, invalid_condition, synq::compiler::HybridQuantumGate{}, {1, 1, 20}});
    auto invalid_tree_resolved = synq::compiler::resolve_hybrid_names(invalid_tree_program);
    expect(!invalid_tree_resolved.ok() && invalid_tree_resolved.diagnostics[0].code == "SYNQ-T002",
           "invalid manually constructed Boolean trees should have a dedicated internal diagnostic");

    auto gated = parser.parseSourceWithDiagnostics("if ready and fallback then quantum h q[0]\n");
    expect(!gated.ok() && gated.diagnostics[0].code == "SYNQ-P007",
           "Boolean operators must retain the existing alpha feature gate");

    return 0;
}
