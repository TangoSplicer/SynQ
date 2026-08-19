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
                                                         std::size_t max_invocations = 128,
                                                         std::size_t max_operations = 128) {
    synq::compiler::BoundedRuntimeEvaluationOptions options;
    options.allow_experimental_runtime_evaluation = true;
    options.max_callable_invocations = max_invocations;
    options.max_operations = max_operations;
    return synq::compiler::evaluate_bounded_runtime(program, options);
}

bool executes_ordered_integer_and_boolean_binary_frames() {
    synq::compiler::ResolvedHybridProgram program;
    synq::compiler::HybridProgram lowered_program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn subtract(left: Integer, right: Integer) -> left - right\n"
        "fn both(left: Boolean, right: Boolean) -> left and right\n"
        "let base = 50\n"
        "let answer = subtract(base, 8)\n"
        "let enabled = both(true, false)\n";
    if (!require(prepare(source, program, &lowered_program), "U6 positive fixture parses, lowers, and resolves")) return false;
    if (!require(lowered_program.nodes.size() == 5 &&
                     std::holds_alternative<synq::compiler::HybridCallableDeclaration>(lowered_program.nodes[0]) &&
                     std::get<synq::compiler::HybridCallableDeclaration>(lowered_program.nodes[0]).binary_classical_body.has_value() &&
                     std::holds_alternative<synq::compiler::HybridDeclaration>(lowered_program.nodes[3]) &&
                     std::get<synq::compiler::HybridDeclaration>(lowered_program.nodes[3]).binary_classical_callable_invocation.has_value(),
                 "ordered U6 declaration and two-actual invocation provenance survives Hybrid lowering")) return false;
    const auto result = evaluate(program);
    if (!require(result.ok() && result.evaluation->bindings.size() == 3,
                 "runtime evaluates U6 immutable result bindings but not callable metadata")) return false;
    const auto& bindings = result.evaluation->bindings;
    return require(bindings[0].name == "base" && bindings[0].value.integer_value == 50 &&
                       bindings[1].name == "answer" && bindings[1].value.integer_value == 42 &&
                       bindings[2].name == "enabled" && !bindings[2].value.boolean_value,
                   "U6 ordered Integer subtraction and Boolean conjunction return deterministic typed values");
}

bool enforces_dual_gates_and_static_boundaries() {
    Parser parser;
    const auto missing_u6_gate = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "fn add(left: Integer, right: Integer) -> left + right\n");
    if (!require(!missing_u6_gate.ok() && has_code(missing_u6_gate.diagnostics, "SYNQ-P007"),
                 "U6 two-formal declaration requires its separate Alpha opt-in")) return false;
    const auto malformed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn capture(left: Integer, right: Integer) -> left + global\n");
    if (!require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P021"),
                 "U6 parser rejects capture and unsupported binary body shapes")) return false;
    const auto malformed_call = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "let answer = add(1, 2, 3)\n");
    if (!require(!malformed_call.ok() && has_code(malformed_call.diagnostics, "SYNQ-P022"),
                 "U6 parser rejects any non-binary invocation arity")) return false;

    const auto call_before_definition = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "let answer = add(20, 22)\n"
        "fn add(left: Integer, right: Integer) -> left + right\n");
    if (!require(call_before_definition.ok(), "U6 call-before-definition fixture is syntactically typed")) return false;
    const auto call_before_lowered = synq::compiler::lower_to_hybrid_ir(*call_before_definition.program);
    const auto call_before_resolved = synq::compiler::resolve_hybrid_names(*call_before_lowered.program);
    if (!require(!call_before_resolved.ok() && has_code(call_before_resolved.diagnostics, "SYNQ-R011"),
                 "resolver requires an earlier U6 binary function definition")) return false;

    const auto mismatch = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn both(left: Boolean, right: Boolean) -> left or right\n"
        "let answer = both(true, 1)\n");
    if (!require(mismatch.ok(), "U6 ordered type-mismatch fixture parses")) return false;
    const auto mismatch_lowered = synq::compiler::lower_to_hybrid_ir(*mismatch.program);
    const auto mismatch_resolved = synq::compiler::resolve_hybrid_names(*mismatch_lowered.program);
    return require(!mismatch_resolved.ok() && has_code(mismatch_resolved.diagnostics, "SYNQ-R012"),
                   "resolver requires exact static types for both ordered U6 actuals");
}

bool enforces_opt_in_bounds_and_atomic_failure() {
    synq::compiler::ResolvedHybridProgram program;
    if (!require(prepare(
                     "#[experimental(feature = \"classical-callable-execution\")]\n"
                     "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
                     "fn add(left: Integer, right: Integer) -> left + right\n"
                     "let first = add(1, 2)\n"
                     "let second = add(3, 4)\n", program),
                 "U6 bound fixture parses, lowers, and resolves")) return false;
    synq::compiler::BoundedRuntimeEvaluationOptions disabled;
    const auto disabled_result = synq::compiler::evaluate_bounded_runtime(program, disabled);
    if (!require(!disabled_result.ok() && !disabled_result.evaluation.has_value() &&
                     has_code(disabled_result.diagnostics, "SYNQ-E011"),
                 "U6 runtime API retains explicit opt-in")) return false;
    const auto invocation_result = evaluate(program, 1);
    if (!require(!invocation_result.ok() && !invocation_result.evaluation.has_value() &&
                     has_code(invocation_result.diagnostics, "SYNQ-E012"),
                 "U6 invocation limit returns no partial result")) return false;
    const auto operation_result = evaluate(program, 128, 0);
    if (!require(!operation_result.ok() && !operation_result.evaluation.has_value() &&
                     has_code(operation_result.diagnostics, "SYNQ-E007"),
                 "U6 checked binary body arithmetic uses the explicit operation budget atomically")) return false;
    synq::compiler::ResolvedHybridProgram overflow_program;
    if (!require(prepare(
                     "#[experimental(feature = \"classical-callable-execution\")]\n"
                     "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
                     "fn add(left: Integer, right: Integer) -> left + right\n"
                     "let overflow = add(9223372036854775807, 1)\n", overflow_program),
                 "U6 overflow fixture parses, lowers, and resolves")) return false;
    const auto overflow_result = evaluate(overflow_program);
    return require(!overflow_result.ok() && !overflow_result.evaluation.has_value() &&
                       has_code(overflow_result.diagnostics, "SYNQ-E005"),
                   "U6 checked binary arithmetic rejects int64 overflow atomically");
}

bool rejects_u6_nodes_in_quantum_paths() {
    synq::compiler::ResolvedHybridProgram resolved_program;
    synq::compiler::HybridProgram lowered_program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn add(left: Integer, right: Integer) -> left + right\n"
        "let answer = add(20, 22)\n";
    if (!require(prepare(source, resolved_program, &lowered_program),
                 "U6 non-runtime-path fixture parses, lowers, and resolves")) return false;
    const auto exported = synq::compiler::export_hybrid_openqasm3(lowered_program);
    if (!require(!exported.ok() && !exported.diagnostics.empty() &&
                     exported.diagnostics.front().find("local classical callable runtime") != std::string::npos,
                 "strict Hybrid export explicitly rejects U6 local callable declarations")) return false;
    synq::compiler::BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulated = synq::compiler::simulate_bounded_quantum(resolved_program, options);
    return require(!simulated.ok() && has_code(simulated.diagnostics, "SYNQ-SIM002"),
                   "quantum simulator explicitly rejects U6 local callable execution nodes");
}

}  // namespace

int main() {
    if (!executes_ordered_integer_and_boolean_binary_frames()) return 1;
    if (!enforces_dual_gates_and_static_boundaries()) return 1;
    if (!enforces_opt_in_bounds_and_atomic_failure()) return 1;
    if (!rejects_u6_nodes_in_quantum_paths()) return 1;
    std::cout << "SynQ bounded binary classical callable runtime smoke test passed\n";
    return 0;
}
