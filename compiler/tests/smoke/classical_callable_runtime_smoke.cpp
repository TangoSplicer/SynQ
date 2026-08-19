#include <iostream>
#include <string>

#include "compiler/bounded_evaluator.h"
#include "compiler/bounded_simulator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/openqasm3_exporter.h"
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

bool prepare(const std::string& source, synq::compiler::ResolvedHybridProgram& resolved_program,
             synq::compiler::HybridProgram* lowered_program = nullptr) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return false;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return false;
    if (lowered_program != nullptr) *lowered_program = *lowered.program;
    resolved_program = *resolved.program;
    return true;
}

synq::compiler::BoundedRuntimeEvaluationResult evaluate(const synq::compiler::ResolvedHybridProgram& program,
                                                         std::size_t max_callables = 32,
                                                         std::size_t max_invocations = 128,
                                                         std::size_t max_depth = 1,
                                                         std::size_t max_expression_depth = 16,
                                                         std::size_t max_operations = 128) {
    synq::compiler::BoundedRuntimeEvaluationOptions options;
    options.allow_experimental_runtime_evaluation = true;
    options.max_callable_declarations = max_callables;
    options.max_callable_invocations = max_invocations;
    options.max_call_depth = max_depth;
    options.max_expression_depth = max_expression_depth;
    options.max_operations = max_operations;
    return synq::compiler::evaluate_bounded_runtime(program, options);
}

bool executes_one_typed_local_frame_per_call() {
    synq::compiler::ResolvedHybridProgram program;
    synq::compiler::HybridProgram lowered_program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn increment(value: Integer) -> value + 1\n"
        "fn invert(value: Boolean) -> not value\n"
        "fn echo(value: String) -> value\n"
        "let base = 41\n"
        "let answer = increment(base)\n"
        "let flipped = invert(true)\n"
        "let label = echo(\"SynQ\")\n";
    if (!require(prepare(source, program, &lowered_program), "U5 positive fixture parses, lowers, and resolves")) return false;
    if (!require(lowered_program.nodes.size() == 7 &&
                     std::holds_alternative<synq::compiler::HybridCallableDeclaration>(lowered_program.nodes[0]) &&
                     std::get<synq::compiler::HybridCallableDeclaration>(lowered_program.nodes[0]).classical_body.has_value() &&
                     std::holds_alternative<synq::compiler::HybridDeclaration>(lowered_program.nodes[4]) &&
                     std::get<synq::compiler::HybridDeclaration>(lowered_program.nodes[4]).classical_callable_invocation.has_value(),
                 "typed U5 declaration and invocation provenance survives Hybrid lowering")) return false;
    const auto result = evaluate(program);
    if (!require(result.ok() && result.evaluation->bindings.size() == 4,
                 "runtime evaluates immutable declarations but not callable metadata")) return false;
    const auto& bindings = result.evaluation->bindings;
    return require(bindings[0].name == "base" && bindings[0].value.integer_value == 41 &&
                       bindings[1].name == "answer" && bindings[1].value.integer_value == 42 &&
                       bindings[2].name == "flipped" && !bindings[2].value.boolean_value &&
                       bindings[3].name == "label" && bindings[3].value.string_value == "SynQ",
                   "one-formal Integer, Boolean, and String functions return deterministic typed values");
}

bool enforces_gate_parse_and_resolution_boundaries() {
    Parser parser;
    const auto missing_gate = parser.parseSourceWithDiagnostics("fn increment(value: Integer) -> value + 1\n");
    if (!require(!missing_gate.ok() && has_code(missing_gate.diagnostics, "SYNQ-P007"),
                 "U5 function syntax requires explicit Alpha opt-in")) return false;
    const auto malformed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn capture(value: Integer) -> value + global\n");
    if (!require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P019"),
                 "U5 parser rejects capture and unsupported function-body shapes")) return false;

    const auto call_before_definition = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "let answer = increment(1)\n"
        "fn increment(value: Integer) -> value + 1\n");
    if (!require(call_before_definition.ok(), "U5 call-before-definition fixture is syntactically typed")) return false;
    const auto call_before_lowered = synq::compiler::lower_to_hybrid_ir(*call_before_definition.program);
    const auto call_before_resolved = synq::compiler::resolve_hybrid_names(*call_before_lowered.program);
    if (!require(!call_before_resolved.ok() && has_code(call_before_resolved.diagnostics, "SYNQ-R009"),
                 "resolver requires an earlier U5 function definition")) return false;

    const auto mismatch = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn invert(value: Boolean) -> not value\n"
        "let answer = invert(1)\n");
    if (!require(mismatch.ok(), "U5 static type-mismatch fixture parses")) return false;
    const auto mismatch_lowered = synq::compiler::lower_to_hybrid_ir(*mismatch.program);
    const auto mismatch_resolved = synq::compiler::resolve_hybrid_names(*mismatch_lowered.program);
    return require(!mismatch_resolved.ok() && has_code(mismatch_resolved.diagnostics, "SYNQ-R010"),
                   "resolver requires an exact U5 callable actual static type");
}

bool enforces_explicit_opt_in_and_atomic_bounds() {
    synq::compiler::ResolvedHybridProgram program;
    if (!require(prepare(
                     "#[experimental(feature = \"classical-callable-execution\")]\n"
                     "fn identity(value: Integer) -> value\n"
                     "let first = identity(1)\n"
                     "let second = identity(2)\n",
                     program),
                 "U5 bound fixture parses, lowers, and resolves")) return false;
    synq::compiler::BoundedRuntimeEvaluationOptions disabled;
    const auto disabled_result = synq::compiler::evaluate_bounded_runtime(program, disabled);
    if (!require(!disabled_result.ok() && !disabled_result.evaluation.has_value() &&
                     has_code(disabled_result.diagnostics, "SYNQ-E011"),
                 "U5 runtime API requires explicit opt-in")) return false;
    const auto invocation_result = evaluate(program, 32, 1);
    if (!require(!invocation_result.ok() && !invocation_result.evaluation.has_value() &&
                     has_code(invocation_result.diagnostics, "SYNQ-E012"),
                 "U5 invocation limit returns no partial result")) return false;

    synq::compiler::ResolvedHybridProgram arithmetic_program;
    if (!require(prepare(
                     "#[experimental(feature = \"classical-callable-execution\")]\n"
                     "fn increment(value: Integer) -> value + 1\n"
                     "let answer = increment(1)\n",
                     arithmetic_program),
                 "U5 operation fixture parses, lowers, and resolves")) return false;
    const auto operation_result = evaluate(arithmetic_program, 32, 128, 1, 16, 0);
    return require(!operation_result.ok() && !operation_result.evaluation.has_value() &&
                       has_code(operation_result.diagnostics, "SYNQ-E007"),
                   "U5 checked body arithmetic uses the explicit operation budget atomically");
}

bool rejects_u5_nodes_in_quantum_paths() {
    synq::compiler::ResolvedHybridProgram resolved_program;
    synq::compiler::HybridProgram lowered_program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn identity(value: Integer) -> value\n"
        "let answer = identity(1)\n";
    if (!require(prepare(source, resolved_program, &lowered_program),
                 "U5 non-runtime-path fixture parses, lowers, and resolves")) return false;
    const auto exported = synq::compiler::export_hybrid_openqasm3(lowered_program);
    if (!require(!exported.ok() && !exported.diagnostics.empty() &&
                     exported.diagnostics.front().find("classical callable runtime") != std::string::npos,
                 "strict Hybrid export explicitly rejects U5 local callable declarations")) return false;
    synq::compiler::BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulated = synq::compiler::simulate_bounded_quantum(resolved_program, options);
    return require(!simulated.ok() && has_code(simulated.diagnostics, "SYNQ-SIM002"),
                   "quantum simulator explicitly rejects U5 local callable execution nodes");
}

}  // namespace

int main() {
    if (!executes_one_typed_local_frame_per_call()) return 1;
    if (!enforces_gate_parse_and_resolution_boundaries()) return 1;
    if (!enforces_explicit_opt_in_and_atomic_bounds()) return 1;
    if (!rejects_u5_nodes_in_quantum_paths()) return 1;
    std::cout << "SynQ bounded classical callable runtime smoke test passed\n";
    return 0;
}
