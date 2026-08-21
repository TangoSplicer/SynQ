#include <iostream>
#include <string>
#include <vector>

#include "compiler/bounded_evaluator.h"
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

bool prepare(const std::string& source, synq::compiler::ResolvedHybridProgram& resolved_program) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return false;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return false;
    resolved_program = *resolved.program;
    return true;
}

synq::compiler::BoundedEvaluationResult evaluate_constants(const synq::compiler::ResolvedHybridProgram& program) {
    synq::compiler::BoundedEvaluationOptions options;
    options.allow_experimental_constant_evaluation = true;
    return synq::compiler::evaluate_bounded_constants(program, options);
}

synq::compiler::BoundedStateEvaluationResult evaluate_state(const synq::compiler::ResolvedHybridProgram& program) {
    synq::compiler::BoundedStateEvaluationOptions options;
    options.allow_experimental_state_evaluation = true;
    return synq::compiler::evaluate_bounded_state(program, options);
}

synq::compiler::BoundedRuntimeEvaluationResult evaluate_runtime(const synq::compiler::ResolvedHybridProgram& program) {
    synq::compiler::BoundedRuntimeEvaluationOptions options;
    options.allow_experimental_runtime_evaluation = true;
    return synq::compiler::evaluate_bounded_runtime(program, options);
}

bool shared_immutable_declarations_agree_across_modes() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "let seed = 20\n"
        "let answer = seed + 22\n";
    if (!require(prepare(source, program), "LCEP-COMP-001A shared immutable source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(constants.ok() && constants.evaluation->bindings.size() == 2 &&
                     constants.evaluation->bindings[1].value.integer_value == 42,
                 "constants mode returns the shared immutable result")) return false;
    if (!require(state.ok() && state.evaluation->cells.empty(),
                 "state mode accepts shared immutable declarations without inventing cells")) return false;
    return require(runtime.ok() && runtime.evaluation->bindings.size() == 2 &&
                       runtime.evaluation->bindings[1].value.integer_value == 42,
                   "runtime mode returns the same shared immutable result");
}

bool mutable_state_is_mode_exclusive() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"mutable-classical-state\")]\n"
        "var counter = 1\n"
        "set counter = 2\n";
    if (!require(prepare(source, program), "LCEP-COMP-001B mutable-state source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects mutable state")) return false;
    if (!require(state.ok() && state.evaluation->cells.size() == 1 &&
                     state.evaluation->cells.front().value.integer_value == 2,
                 "state mode evaluates final mutable-cell state")) return false;
    return require(!runtime.ok() && !runtime.evaluation.has_value() && has_code(runtime.diagnostics, "SYNQ-E014"),
                   "runtime mode atomically rejects mutable state");
}

bool local_callables_are_mode_exclusive() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn increment(value: Integer) -> value + 1\n"
        "fn add(left: Integer, right: Integer) -> left + right\n"
        "let first = increment(20)\n"
        "let answer = add(first, 21)\n";
    if (!require(prepare(source, program), "LCEP-COMP-001C U5/U6 source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects local callable declarations")) return false;
    if (!require(!state.ok() && !state.evaluation.has_value() && has_code(state.diagnostics, "SYNQ-E010"),
                 "state mode atomically rejects local callable declarations")) return false;
    return require(runtime.ok() && runtime.evaluation->bindings.size() == 2 &&
                       runtime.evaluation->bindings[0].value.integer_value == 21 &&
                       runtime.evaluation->bindings[1].value.integer_value == 42,
                   "runtime mode evaluates ordered U5 then U6 immutable result bindings");
}

bool state_and_callable_source_has_no_combined_evaluator() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"mutable-classical-state\")]\n"
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "var counter = 1\n"
        "fn increment(value: Integer) -> value + 1\n"
        "let answer = increment(41)\n";
    if (!require(prepare(source, program), "LCEP-COMP-001D mixed state/callable source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects the mixed state/callable source")) return false;
    if (!require(!state.ok() && !state.evaluation.has_value() && has_code(state.diagnostics, "SYNQ-E010"),
                 "state mode rejects callable composition without a partial cell result")) return false;
    return require(!runtime.ok() && !runtime.evaluation.has_value() && has_code(runtime.diagnostics, "SYNQ-E014"),
                   "runtime mode rejects state composition without a partial binding result");
}

bool quantum_nodes_remain_outside_all_local_modes() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "let seed = 1\n"
        "qubit q[1]\n"
        "quantum h q[0]\n";
    if (!require(prepare(source, program), "LCEP-COMP-001E hybrid source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects quantum nodes")) return false;
    if (!require(!state.ok() && !state.evaluation.has_value() && has_code(state.diagnostics, "SYNQ-E010"),
                 "state mode atomically rejects quantum nodes")) return false;
    return require(!runtime.ok() && !runtime.evaluation.has_value() && has_code(runtime.diagnostics, "SYNQ-E014"),
                   "runtime mode atomically rejects quantum nodes");
}

}  // namespace

int main() {
    if (!shared_immutable_declarations_agree_across_modes()) return 1;
    if (!mutable_state_is_mode_exclusive()) return 1;
    if (!local_callables_are_mode_exclusive()) return 1;
    if (!state_and_callable_source_has_no_combined_evaluator()) return 1;
    if (!quantum_nodes_remain_outside_all_local_modes()) return 1;
    std::cout << "SynQ local classical evaluation composition smoke test passed\n";
    return 0;
}
