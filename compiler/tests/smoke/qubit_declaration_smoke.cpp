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

bool deliberately_does_not_validate_operand_bounds_yet() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "qubit q[1]\n"
        "quantum h q[8]\n"
        "measure q[4]\n");
    if (!require(parsed.ok(), "declarations do not yet change existing indexed operand parsing")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    return require(lowered.ok() && resolved.ok(),
                   "qubit declaration increment intentionally defers gate and measurement bound validation");
}

}  // namespace

int main() {
    if (!parses_lowers_and_resolves_qubit_declarations()) return 1;
    if (!enforces_gate_grammar_and_shared_top_level_names()) return 1;
    if (!deliberately_does_not_validate_operand_bounds_yet()) return 1;
    std::cout << "SynQ bounded qubit declaration smoke test passed\n";
    return 0;
}
