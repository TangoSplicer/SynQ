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

bool parses_lowers_and_resolves_bounded_integer_arithmetic() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "let first = 7\n"
        "let second = -2\n"
        "let total = first + second\n"
        "let product = total * 3\n");
    if (!require(parsed.ok(), "Alpha-gated integer arithmetic fixture parses")) return false;

    const auto* total = dynamic_cast<const DeclarationNode*>(parsed.program->statements.at(2));
    if (!require(total != nullptr && total->literal_kind == ClassicalLiteralKind::IntegerArithmeticExpression,
                 "Alpha-gated parser provenance marks the bounded arithmetic declaration")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "arithmetic fixture lowers to Hybrid IR")) return false;
    const auto* total_declaration = std::get_if<synq::compiler::HybridDeclaration>(&lowered.program->nodes.at(2));
    if (!require(total_declaration != nullptr &&
                     total_declaration->initializer.kind == synq::compiler::ClassicalExpressionKind::IntegerArithmeticExpression &&
                     total_declaration->initializer.static_type == synq::compiler::ClassicalStaticType::Integer &&
                     total_declaration->initializer.integer_arithmetic.has_value(),
                 "Hybrid IR preserves a typed Integer arithmetic initializer")) return false;
    if (!require(total_declaration->initializer.integer_arithmetic->kind ==
                     ClassicalIntegerArithmeticExpressionKind::Add &&
                     total_declaration->initializer.integer_arithmetic->operands.size() == 2,
                 "Hybrid IR preserves the explicit binary Add shape")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "integer arithmetic operands resolve without evaluation")) return false;
    const auto* resolved_total = std::get_if<synq::compiler::ResolvedHybridDeclaration>(&resolved.program->nodes.at(2));
    const auto* resolved_product = std::get_if<synq::compiler::ResolvedHybridDeclaration>(&resolved.program->nodes.at(3));
    if (!require(resolved_total != nullptr &&
                     resolved_total->initializer_binding_indices == std::vector<std::size_t>({0, 1}) &&
                     resolved_total->initializer_static_type == synq::compiler::ClassicalStaticType::Integer,
                 "resolver retains source-ordered Integer operand binding indices")) return false;
    return require(resolved_product != nullptr &&
                       resolved_product->initializer_binding_indices == std::vector<std::size_t>({2}) &&
                       resolved_product->initializer_static_type == synq::compiler::ClassicalStaticType::Integer,
                   "arithmetic declaration can be a later Integer operand without computation");
}

bool retains_featureless_source_and_rejects_malformed_opted_in_arithmetic() {
    Parser parser;
    const auto featureless = parser.parseSourceWithDiagnostics("let total = 1 + 2\n");
    if (!require(featureless.ok(), "featureless arithmetic-looking source remains parser-compatible")) return false;
    const auto featureless_lowered = synq::compiler::lower_to_hybrid_ir(*featureless.program);
    const auto* featureless_declaration = std::get_if<synq::compiler::HybridDeclaration>(&featureless_lowered.program->nodes.front());
    if (!require(featureless_lowered.ok() && featureless_declaration != nullptr &&
                     featureless_declaration->initializer.kind == synq::compiler::ClassicalExpressionKind::OpaqueSource,
                 "Alpha opt-in gates typed arithmetic promotion rather than rejecting prior opaque source")) return false;

    const auto malformed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "let total = 1 + 2 + 3\n");
    return require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P011"),
                   "parser rejects nested or repeated arithmetic operators");
}

bool rejects_unknown_non_integer_and_invalid_arithmetic_operands() {
    Parser parser;
    const auto unknown_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "let total = missing + 1\n");
    if (!require(unknown_parsed.ok(), "unknown arithmetic identifier remains a resolver concern")) return false;
    const auto unknown_lowered = synq::compiler::lower_to_hybrid_ir(*unknown_parsed.program);
    const auto unknown_resolved = synq::compiler::resolve_hybrid_names(*unknown_lowered.program);
    if (!require(!unknown_resolved.ok() && has_code(unknown_resolved.diagnostics, "SYNQ-R003"),
                 "unknown arithmetic operand reports SYNQ-R003")) return false;

    const auto boolean_parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "let ready = true\n"
        "let total = ready + 1\n");
    if (!require(boolean_parsed.ok(), "non-Integer arithmetic fixture parses before static resolution")) return false;
    const auto boolean_lowered = synq::compiler::lower_to_hybrid_ir(*boolean_parsed.program);
    const auto boolean_resolved = synq::compiler::resolve_hybrid_names(*boolean_lowered.program);
    if (!require(!boolean_resolved.ok() && has_code(boolean_resolved.diagnostics, "SYNQ-T003"),
                 "non-Integer arithmetic operand reports SYNQ-T003")) return false;

    ClassicalIntegerArithmeticExpression malformed_tree{
        ClassicalIntegerArithmeticExpressionKind::Add, "broken", {1, 1, 7},
        {{ClassicalIntegerArithmeticExpressionKind::IntegerLiteral, "1", {1, 1, 2}, {}}}};
    synq::compiler::ClassicalExpression initializer;
    initializer.kind = synq::compiler::ClassicalExpressionKind::IntegerArithmeticExpression;
    initializer.static_type = synq::compiler::ClassicalStaticType::Integer;
    initializer.source_text = "broken";
    initializer.span = {1, 1, 7};
    initializer.integer_arithmetic = malformed_tree;
    synq::compiler::HybridProgram invalid_program;
    invalid_program.nodes.emplace_back(synq::compiler::HybridDeclaration{
        "broken", "broken", ClassicalLiteralKind::SourceText, initializer, {1, 1, 7}});
    const auto invalid_resolved = synq::compiler::resolve_hybrid_names(invalid_program);
    return require(!invalid_resolved.ok() && has_code(invalid_resolved.diagnostics, "SYNQ-T004"),
                   "invalid internal arithmetic tree reports SYNQ-T004");
}

}  // namespace

int main() {
    if (!parses_lowers_and_resolves_bounded_integer_arithmetic()) return 1;
    if (!retains_featureless_source_and_rejects_malformed_opted_in_arithmetic()) return 1;
    if (!rejects_unknown_non_integer_and_invalid_arithmetic_operands()) return 1;
    std::cout << "SynQ bounded integer arithmetic expression smoke test passed\n";
    return 0;
}
