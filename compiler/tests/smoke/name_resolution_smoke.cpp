// Scoped-name-resolution smoke coverage for the minimal Hybrid IR boundary.
#include <cstdlib>
#include <iostream>
#include <variant>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "name-resolution smoke failure: " << message << '\n';
        std::exit(1);
    }
}

synq::compiler::HybridProgram lower_successfully(const std::string& source) {
    Parser parser;
    auto parsed = parser.parseSourceWithDiagnostics(source);
    expect(parsed.ok(), "fixture should parse through the recovery profile");
    auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    expect(lowered.ok(), "fixture should lower through the minimal Hybrid IR");
    return std::move(*lowered.program);
}

}  // namespace

int main() {
    const auto resolved_input = lower_successfully(
        "let shots = 1024\n"
        "let selected = shots\n"
        "quantum h q[0]\n"
        "measure q[0]\n");
    auto resolved = synq::compiler::resolve_hybrid_names(resolved_input);
    expect(resolved.ok(), "an initializer should resolve to an earlier top-level declaration");
    expect(resolved.program->nodes.size() == 4, "resolution should preserve node count and order");

    const auto* first_declaration =
        std::get_if<synq::compiler::ResolvedHybridDeclaration>(&resolved.program->nodes[0]);
    const auto* second_declaration =
        std::get_if<synq::compiler::ResolvedHybridDeclaration>(&resolved.program->nodes[1]);
    expect(first_declaration != nullptr && second_declaration != nullptr,
           "declarations should remain typed after resolution");
    expect(!first_declaration->initializer_binding_index.has_value(),
           "integer literal initializer should not fabricate a binding reference");
    expect(second_declaration->initializer_binding_index == std::optional<std::size_t>{0},
           "whole-identifier initializer should bind to the earlier declaration index");
    expect(second_declaration->declaration.span.line == 2,
           "resolved declaration should retain parser source provenance");
    expect(std::holds_alternative<synq::compiler::HybridQuantumGate>(resolved.program->nodes[2]),
           "resolution should preserve typed quantum gates unchanged");
    expect(std::holds_alternative<synq::compiler::HybridMeasurement>(resolved.program->nodes[3]),
           "resolution should preserve typed measurements unchanged");

    const auto opaque_source_text = lower_successfully("let expression = one + two\n");
    auto opaque_resolution = synq::compiler::resolve_hybrid_names(opaque_source_text);
    expect(opaque_resolution.ok(), "unparsed source text should remain opaque rather than receive invented expression semantics");
    const auto* opaque_declaration =
        std::get_if<synq::compiler::ResolvedHybridDeclaration>(&opaque_resolution.program->nodes[0]);
    expect(opaque_declaration != nullptr && !opaque_declaration->initializer_binding_index.has_value(),
           "opaque source text should not resolve as a whole-identifier reference");

    const auto unknown_input = lower_successfully("let selected = missing\n");
    auto unknown = synq::compiler::resolve_hybrid_names(unknown_input);
    expect(!unknown.ok(), "unknown whole-identifier initializer should be rejected");
    expect(unknown.program == std::nullopt, "failed name resolution should not return a partial program");
    expect(unknown.diagnostics.size() == 1 && unknown.diagnostics[0].code == "SYNQ-R001",
           "unknown binding should use a stable name-resolution diagnostic code");
    expect(unknown.diagnostics[0].span.line == 1,
           "unknown binding diagnostic should retain declaration provenance");

    const auto forward_input = lower_successfully(
        "let selected = shots\n"
        "let shots = 1024\n");
    auto forward = synq::compiler::resolve_hybrid_names(forward_input);
    expect(!forward.ok(), "forward binding should be rejected in the first sequential scope");
    expect(forward.diagnostics.size() == 1 && forward.diagnostics[0].code == "SYNQ-R001",
           "forward binding should share the explicit unresolved-binding diagnostic");
    expect(forward.diagnostics[0].span.line == 1,
           "forward binding diagnostic should identify the referring declaration");

    return 0;
}
