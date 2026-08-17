#include <cstdlib>
#include <iostream>
#include <string>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

void expect(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "semantic-kernel smoke failure: " << message << '\n';
        std::exit(1);
    }
}

synq::compiler::ResolvedHybridProgram resolve_successfully(const std::string& source) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    expect(parsed.ok(), "fixture should parse through the bounded recovery profile");
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    expect(lowered.ok(), "fixture should lower to Hybrid IR");
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    expect(resolved.ok(), "fixture should resolve in the top-level semantic environment");
    return std::move(*resolved.program);
}

}  // namespace

int main() {
    const auto resolved = resolve_successfully(
        "let seed = 5\n"
        "let selected = seed\n"
        "let label = \"ready\"\n"
        "measure q[0] as observed\n");

    expect(resolved.semantic_bindings.size() == 4,
           "semantic environment records declarations and named measurement results in source order");
    const auto& seed = resolved.semantic_bindings[0];
    const auto& selected = resolved.semantic_bindings[1];
    const auto& label = resolved.semantic_bindings[2];
    const auto& observed = resolved.semantic_bindings[3];
    expect(seed.name == "seed" && seed.kind == synq::compiler::SemanticBindingKind::Value &&
               seed.static_type == synq::compiler::ClassicalStaticType::Integer && seed.span.line == 1,
           "literal declaration has a stable immutable Integer binding record");
    expect(selected.name == "selected" && selected.static_type == synq::compiler::ClassicalStaticType::Integer &&
               selected.dependency_names.size() == 1 && selected.dependency_names[0] == "seed",
           "identifier alias inherits the earlier static type and records its dependency name");
    expect(label.static_type == synq::compiler::ClassicalStaticType::String,
           "quoted string declaration preserves its bounded static type");
    expect(observed.kind == synq::compiler::SemanticBindingKind::MeasurementResult &&
               observed.static_type == synq::compiler::ClassicalStaticType::Boolean && observed.span.line == 4,
           "named measurement result is descriptive Boolean metadata without an invented runtime value");

    const std::string rendered = synq::compiler::render_semantic_environment(resolved);
    expect(rendered.find("semantic environment: top-level immutable bindings") != std::string::npos &&
               rendered.find("binding selected | Value | Integer | line 2 | depends-on seed") != std::string::npos &&
               rendered.find("binding observed | MeasurementResult | Boolean | line 4") != std::string::npos,
           "inspection renderer reports binding kind, static type, provenance, and dependencies deterministically");

    const auto opaque = resolve_successfully("let unresolved_shape = one + two\n");
    expect(opaque.semantic_bindings.size() == 1 &&
               opaque.semantic_bindings.front().static_type == synq::compiler::ClassicalStaticType::Unknown,
           "opaque source text remains explicitly Unknown instead of receiving invented semantic meaning");

    std::cout << "SynQ alpha semantic-kernel smoke test passed\n";
    return 0;
}
