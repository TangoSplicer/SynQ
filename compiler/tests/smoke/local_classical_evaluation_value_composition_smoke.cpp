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

bool immutable_values_are_consistent_where_exposed() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let seed = 12\n"
        "let total = seed + 3\n"
        "let enabled = true\n"
        "let disabled = not enabled\n"
        "let title = \"SynQ\"\n";
    if (!require(prepare(source, program), "LCEP-COMP-002A multi-type immutable source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(constants.ok() && constants.evaluation->bindings.size() == 5 &&
                     constants.evaluation->bindings[1].value.integer_value == 15 &&
                     constants.evaluation->bindings[2].value.boolean_value &&
                     !constants.evaluation->bindings[3].value.boolean_value &&
                     constants.evaluation->bindings[4].value.string_value == "SynQ",
                 "constants mode exposes deterministic Integer, Boolean, and String immutable values")) return false;
    if (!require(state.ok() && state.evaluation->cells.empty(),
                 "state mode accepts immutable values without exposing invented mutable cells")) return false;
    return require(runtime.ok() && runtime.evaluation->bindings.size() == 5 &&
                       runtime.evaluation->bindings[1].value.integer_value == 15 &&
                       runtime.evaluation->bindings[2].value.boolean_value &&
                       !runtime.evaluation->bindings[3].value.boolean_value &&
                       runtime.evaluation->bindings[4].value.string_value == "SynQ",
                   "runtime mode exposes the same ordered multi-type immutable values as constants mode");
}

bool typed_state_remains_mode_exclusive() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"mutable-classical-state\")]\n"
        "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let seed = 4\n"
        "var count = seed\n"
        "set count = count + 3\n"
        "var armed = true\n"
        "set armed = not armed\n"
        "var title = \"SynQ\"\n"
        "set title = \"ready\"\n";
    if (!require(prepare(source, program), "LCEP-COMP-002B typed state source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects typed mutable state")) return false;
    if (!require(state.ok() && state.evaluation->cells.size() == 3 &&
                     state.evaluation->cells[0].value.integer_value == 7 &&
                     !state.evaluation->cells[1].value.boolean_value &&
                     state.evaluation->cells[2].value.string_value == "ready",
                 "state mode returns final Integer, Boolean, and String cells")) return false;
    return require(!runtime.ok() && !runtime.evaluation.has_value() && has_code(runtime.diagnostics, "SYNQ-E014"),
                   "runtime mode atomically rejects typed mutable state");
}

bool typed_callables_remain_mode_exclusive() {
    synq::compiler::ResolvedHybridProgram program;
    const std::string source =
        "#[experimental(feature = \"classical-callable-execution\")]\n"
        "#[experimental(feature = \"multi-formal-classical-callables\")]\n"
        "fn increment(value: Integer) -> value + 1\n"
        "fn invert(value: Boolean) -> not value\n"
        "fn echo(value: String) -> value\n"
        "fn both(left: Boolean, right: Boolean) -> left and right\n"
        "let answer = increment(41)\n"
        "let flipped = invert(true)\n"
        "let label = echo(\"SynQ\")\n"
        "let enabled = both(true, false)\n";
    if (!require(prepare(source, program), "LCEP-COMP-002C typed callable source prepares")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 "constants mode atomically rejects typed local callables")) return false;
    if (!require(!state.ok() && !state.evaluation.has_value() && has_code(state.diagnostics, "SYNQ-E010"),
                 "state mode atomically rejects typed local callables")) return false;
    return require(runtime.ok() && runtime.evaluation->bindings.size() == 4 &&
                       runtime.evaluation->bindings[0].value.integer_value == 42 &&
                       !runtime.evaluation->bindings[1].value.boolean_value &&
                       runtime.evaluation->bindings[2].value.string_value == "SynQ" &&
                       !runtime.evaluation->bindings[3].value.boolean_value,
                   "runtime mode returns deterministic U5 Integer/Boolean/String and U6 Boolean values");
}

}  // namespace

int main() {
    if (!immutable_values_are_consistent_where_exposed()) return 1;
    if (!typed_state_remains_mode_exclusive()) return 1;
    if (!typed_callables_remain_mode_exclusive()) return 1;
    std::cout << "SynQ local classical evaluation value-composition smoke test passed\n";
    return 0;
}
