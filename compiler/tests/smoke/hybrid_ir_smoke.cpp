// Hybrid IR smoke coverage for the bounded recovery parser profile.
#include <cstdlib>
#include <iostream>
#include <variant>

#include "compiler/hybrid_ir.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "hybrid IR smoke failure: " << message << '\n';
        std::exit(1);
    }
}

}  // namespace

int main() {
    Parser parser;
    auto mixed_parse = parser.parseSourceWithDiagnostics(
        "let shots = 1024\n"
        "quantum h q[0]\n"
        "measure q[0]\n");
    expect(mixed_parse.ok(), "bounded mixed source should parse");

    auto mixed_hir = synq::compiler::lower_to_hybrid_ir(*mixed_parse.program);
    expect(mixed_hir.ok(), "typed recovery nodes should lower to Hybrid IR");
    expect(mixed_hir.program->nodes.size() == 3, "Hybrid IR should preserve statement count and order");

    const auto* declaration = std::get_if<synq::compiler::HybridDeclaration>(&mixed_hir.program->nodes[0]);
    expect(declaration != nullptr, "first Hybrid IR node should be a declaration");
    expect(declaration->name == "shots", "declaration name should be preserved");
    expect(declaration->source_value == "1024", "declaration source value should be preserved");
    expect(declaration->literal_kind == ClassicalLiteralKind::Integer, "declaration literal hint should be preserved");
    expect(declaration->span.line == 1, "declaration provenance line should be preserved");

    const auto* gate = std::get_if<synq::compiler::HybridQuantumGate>(&mixed_hir.program->nodes[1]);
    expect(gate != nullptr, "second Hybrid IR node should be a quantum gate");
    expect(gate->kind == QuantumGateKind::H, "quantum gate kind should be preserved");
    expect(gate->qubit_indices == std::vector<std::size_t>{0}, "quantum operands should be preserved");
    expect(gate->span.line == 2, "quantum provenance line should be preserved");

    const auto* measurement = std::get_if<synq::compiler::HybridMeasurement>(&mixed_hir.program->nodes[2]);
    expect(measurement != nullptr, "third Hybrid IR node should be a measurement");
    expect(measurement->qubit_index == 0, "measurement qubit should be preserved");
    expect(measurement->span.line == 3, "measurement provenance line should be preserved");

    auto parameterized_parse = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "quantum rx(pi/2) q[1]\n");
    expect(parameterized_parse.ok(), "feature-gated parameterized source should parse");
    auto parameterized_hir = synq::compiler::lower_to_hybrid_ir(*parameterized_parse.program);
    expect(parameterized_hir.ok(), "parameterized typed gate should lower to Hybrid IR");
    const auto* parameterized_gate = std::get_if<synq::compiler::HybridQuantumGate>(&parameterized_hir.program->nodes[0]);
    expect(parameterized_gate != nullptr, "parameterized Hybrid IR node should be a quantum gate");
    expect(parameterized_gate->kind == QuantumGateKind::Rx, "parameterized gate kind should be preserved");
    expect(parameterized_gate->literal_angle.has_value() && *parameterized_gate->literal_angle == "pi/2",
           "literal angle text should be preserved without evaluation");
    expect(parameterized_gate->qubit_indices == std::vector<std::size_t>{1},
           "parameterized gate operand should be preserved");
    expect(parameterized_gate->span.line == 2, "parameterized gate provenance should retain source line");

    auto legacy_parse = parser.parseSourceWithDiagnostics("print recovery-only\n");
    expect(legacy_parse.ok(), "legacy recovery instruction should still parse before Hybrid IR lowering");
    auto legacy_hir = synq::compiler::lower_to_hybrid_ir(*legacy_parse.program);
    expect(!legacy_hir.ok(), "unsupported legacy instruction should not receive invented Hybrid IR semantics");
    expect(legacy_hir.program == std::nullopt, "failed Hybrid IR lowering should not return a partial program");
    expect(legacy_hir.diagnostics.size() == 1, "unsupported legacy instruction should yield one diagnostic");
    expect(legacy_hir.diagnostics[0].code == "SYNQ-H001", "unsupported legacy instruction diagnostic code should be stable");
    expect(legacy_hir.diagnostics[0].span.line == 1, "Hybrid IR rejection should preserve source provenance");

    return 0;
}
