// Alpha-gated bounded classical-control-flow smoke coverage.
#include <cstdlib>
#include <iostream>
#include <variant>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "classical-control-flow smoke failure: " << message << '\n';
        std::exit(1);
    }
}

}  // namespace

int main() {
    Parser parser;
    auto ungated = parser.parseSourceWithDiagnostics("if true then quantum h q[0]\n");
    expect(!ungated.ok(), "classical control flow should require an Alpha opt-in");
    expect(ungated.diagnostics.size() == 1 && ungated.diagnostics[0].code == "SYNQ-P007",
           "missing control-flow opt-in should use the existing feature diagnostic code");

    auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "if true then quantum h q[0]\n"
        "while false do measure q[0]\n");
    expect(parsed.ok(), "gated literal-boolean control flow should parse");
    expect(parsed.program->statements.size() == 2, "feature annotation should not create an AST statement");

    const auto* if_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[0]);
    const auto* while_node = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements[1]);
    expect(if_node != nullptr && while_node != nullptr, "control statements should have typed AST nodes");
    expect(if_node->kind == ClassicalControlKind::If && if_node->condition,
           "if node should preserve the typed true condition");
    expect(while_node->kind == ClassicalControlKind::While && !while_node->condition,
           "while node should preserve the typed false condition");
    expect(dynamic_cast<const QuantumGateNode*>(if_node->body) != nullptr,
           "if body should be a typed quantum gate");
    expect(dynamic_cast<const MeasurementNode*>(while_node->body) != nullptr,
           "while body should be a typed measurement");
    expect(if_node->span.line == 2 && while_node->span.line == 3,
           "control nodes should retain source provenance");

    auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    expect(lowered.ok(), "typed control nodes should lower to the minimal Hybrid IR");
    expect(lowered.program->nodes.size() == 2, "Hybrid IR should retain control-flow order");
    const auto* if_hir = std::get_if<synq::compiler::HybridControlFlow>(&lowered.program->nodes[0]);
    const auto* while_hir = std::get_if<synq::compiler::HybridControlFlow>(&lowered.program->nodes[1]);
    expect(if_hir != nullptr && while_hir != nullptr, "Hybrid IR should hold typed control-flow nodes");
    expect(std::holds_alternative<synq::compiler::HybridQuantumGate>(if_hir->body),
           "if Hybrid IR body should preserve its quantum gate");
    expect(std::holds_alternative<synq::compiler::HybridMeasurement>(while_hir->body),
           "while Hybrid IR body should preserve its measurement");

    auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    expect(resolved.ok(), "bounded name resolution should preserve control nodes unchanged");
    expect(std::holds_alternative<synq::compiler::HybridControlFlow>(resolved.program->nodes[0]) &&
               std::holds_alternative<synq::compiler::HybridControlFlow>(resolved.program->nodes[1]),
           "resolved Hybrid IR should retain control nodes unchanged");

    auto malformed_condition = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "if enabled then quantum h q[0]\n");
    expect(!malformed_condition.ok(), "identifier condition should be rejected in literal-boolean profile");
    expect(malformed_condition.diagnostics.size() == 1 && malformed_condition.diagnostics[0].code == "SYNQ-P009",
           "malformed condition should have a dedicated bounded-control diagnostic");

    auto malformed_body = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "while true do print recovery-only\n");
    expect(!malformed_body.ok(), "non-quantum/non-measurement control body should be rejected");
    expect(malformed_body.diagnostics.size() == 1 && malformed_body.diagnostics[0].code == "SYNQ-P010",
           "malformed body should have a dedicated bounded-control diagnostic");

    return 0;
}
