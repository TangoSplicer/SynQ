#include <iostream>
#include <string>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool has_code(const std::vector<synq::compiler::Diagnostic>& diagnostics, const std::string& code) {
    return diagnostics.size() == 1 && diagnostics.front().code == code;
}

bool parses_lowers_and_resolves_qubit_declarations() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "qubit register[3]\n"
        "quantum h q[2]\n"
        "measure q[0]\n");
    if (!require(parsed.ok(), "Alpha-gated positive-size qubit declaration parses")) return false;

    const auto* declaration = dynamic_cast<const QubitDeclarationNode*>(parsed.program->statements.at(0));
    if (!require(declaration != nullptr && declaration->name == "register" && declaration->qubit_count == 3 &&
                     declaration->span.line == 2,
                 "typed qubit declaration preserves name, size, and source span")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "qubit declaration fixture lowers to Hybrid IR")) return false;
    const auto* hybrid_declaration = std::get_if<synq::compiler::HybridQubitDeclaration>(&lowered.program->nodes.at(0));
    if (!require(hybrid_declaration != nullptr && hybrid_declaration->name == "register" &&
                     hybrid_declaration->qubit_count == 3 && hybrid_declaration->span.line == 2,
                 "Hybrid IR preserves bounded qubit allocation metadata")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "name resolver preserves qubit declarations without allocation semantics")) return false;
    const auto* resolved_declaration = std::get_if<synq::compiler::HybridQubitDeclaration>(&resolved.program->nodes.at(0));
    return require(resolved_declaration != nullptr && resolved_declaration->qubit_count == 3,
                   "resolved Hybrid IR retains qubit declaration metadata");
}

bool enforces_gate_grammar_and_shared_top_level_names() {
    Parser parser;
    const auto featureless = parser.parseSourceWithDiagnostics("qubit q[1]\n");
    if (!require(!featureless.ok() && has_code(featureless.diagnostics, "SYNQ-P007"),
                 "qubit declarations require explicit Alpha opt-in")) return false;

    const auto zero_size = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\nqubit q[0]\n");
    if (!require(!zero_size.ok() && has_code(zero_size.diagnostics, "SYNQ-P012"),
                 "qubit declarations reject a zero-size register")) return false;

    const auto malformed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\nqubit 9q[2]\n");
    if (!require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P012"),
                 "qubit declarations reject invalid identifiers")) return false;

    const auto duplicate = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\nqubit q[2]\nlet q = 1\n");
    return require(!duplicate.ok() && has_code(duplicate.diagnostics, "SYNQ-S004"),
                   "qubit declarations participate in existing top-level uniqueness checks");
}

bool validates_default_register_operand_order_and_bounds() {
    Parser parser;
    const auto out_of_range_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "qubit q[1]\n"
        "quantum h q[8]\n"
        "measure q[4]\n");
    if (!require(out_of_range_parsed.ok(), "out-of-range operand fixture remains a successful parse")) return false;
    const auto out_of_range_lowered = synq::compiler::lower_to_hybrid_ir(*out_of_range_parsed.program);
    const auto out_of_range_resolved = synq::compiler::resolve_hybrid_names(*out_of_range_lowered.program);
    if (!require(!out_of_range_resolved.ok() && has_code(out_of_range_resolved.diagnostics, "SYNQ-Q002"),
                 "gate operand outside explicit default register reports SYNQ-Q002")) return false;

    const auto forward_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "quantum h q[0]\n"
        "qubit q[1]\n");
    if (!require(forward_parsed.ok(), "forward default-register fixture remains a successful parse")) return false;
    const auto forward_lowered = synq::compiler::lower_to_hybrid_ir(*forward_parsed.program);
    const auto forward_resolved = synq::compiler::resolve_hybrid_names(*forward_lowered.program);
    if (!require(!forward_resolved.ok() && has_code(forward_resolved.diagnostics, "SYNQ-Q001"),
                 "q[index] before explicit default register reports SYNQ-Q001")) return false;

    const auto valid_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[2]\n"
        "quantum cx q[0], q[1]\n"
        "measure q[1]\n"
        "if true then quantum h q[0]\n");
    if (!require(valid_parsed.ok(), "in-range gate, measurement, and control-body fixture parses")) return false;
    const auto valid_lowered = synq::compiler::lower_to_hybrid_ir(*valid_parsed.program);
    const auto valid_resolved = synq::compiler::resolve_hybrid_names(*valid_lowered.program);
    if (!require(valid_lowered.ok() && valid_resolved.ok(),
                 "in-range default-register operands resolve without execution")) return false;

    const auto control_out_of_range_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "if true then quantum h q[1]\n");
    if (!require(control_out_of_range_parsed.ok(), "out-of-range control body remains a successful parse")) return false;
    const auto control_out_of_range_lowered = synq::compiler::lower_to_hybrid_ir(*control_out_of_range_parsed.program);
    const auto control_out_of_range_resolved = synq::compiler::resolve_hybrid_names(*control_out_of_range_lowered.program);
    if (!require(!control_out_of_range_resolved.ok() && has_code(control_out_of_range_resolved.diagnostics, "SYNQ-Q002"),
                 "out-of-range control-body operand reports SYNQ-Q002")) return false;

    const auto legacy_parsed = parser.parseSourceWithDiagnostics("quantum h q[8]\nmeasure q[4]\n");
    if (!require(legacy_parsed.ok(), "legacy indexed operands without a declaration parse")) return false;
    const auto legacy_lowered = synq::compiler::lower_to_hybrid_ir(*legacy_parsed.program);
    const auto legacy_resolved = synq::compiler::resolve_hybrid_names(*legacy_lowered.program);
    return require(legacy_lowered.ok() && legacy_resolved.ok(),
                   "legacy operands retain prior behavior when no explicit default register exists");
}

}  // namespace

int main() {
    if (!parses_lowers_and_resolves_qubit_declarations()) return 1;
    if (!enforces_gate_grammar_and_shared_top_level_names()) return 1;
    if (!validates_default_register_operand_order_and_bounds()) return 1;
    std::cout << "SynQ bounded qubit declaration smoke test passed\n";
    return 0;
}
