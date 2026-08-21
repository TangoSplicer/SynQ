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

bool every_local_mode_rejects_atomically(const std::string& source, const std::string& label) {
    synq::compiler::ResolvedHybridProgram program;
    if (!require(prepare(source, program), label + " prepares through parser, lowering, and resolution")) return false;

    const auto constants = evaluate_constants(program);
    const auto state = evaluate_state(program);
    const auto runtime = evaluate_runtime(program);
    if (!require(!constants.ok() && !constants.evaluation.has_value() && has_code(constants.diagnostics, "SYNQ-E002"),
                 label + " is atomically rejected by constants mode")) return false;
    if (!require(!state.ok() && !state.evaluation.has_value() && has_code(state.diagnostics, "SYNQ-E010"),
                 label + " is atomically rejected by state mode")) return false;
    return require(!runtime.ok() && !runtime.evaluation.has_value() && has_code(runtime.diagnostics, "SYNQ-E014"),
                   label + " is atomically rejected by runtime mode");
}

bool rejects_each_remaining_quantum_and_control_category() {
    const std::string qubit_prelude =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "let marker = 1\n";
    if (!every_local_mode_rejects_atomically(qubit_prelude + "qubit q[1]\n", "LCEP-COMP-003A qubit declaration")) return false;
    if (!every_local_mode_rejects_atomically(qubit_prelude + "qubit q[1]\nquantum h q[0]\n",
                                             "LCEP-COMP-003B direct quantum gate")) return false;

    const std::string measurement_prelude =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "#[experimental(feature = \"measurement-feedback\")]\n"
        "let marker = 1\n"
        "qubit q[1]\n";
    if (!every_local_mode_rejects_atomically(measurement_prelude + "measure q[0] as observed\n",
                                             "LCEP-COMP-003C standalone measurement")) return false;

    const std::string control_prelude =
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let marker = 1\n";
    if (!every_local_mode_rejects_atomically(control_prelude + "if true then quantum h q[0]\n",
                                             "LCEP-COMP-003D control with gate body")) return false;
    if (!every_local_mode_rejects_atomically(control_prelude + "while false do measure q[0]\n",
                                             "LCEP-COMP-003E control with measurement body")) return false;

    const std::string routine_prelude =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"named-qubit-register-operands\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "#[experimental(feature = \"parameterized-quantum-routines\")]\n"
        "let marker = 1\n"
        "qubit q[1]\n"
        "kernel flip(qubit target) { quantum x target }\n"
        "call flip(q[0])\n";
    return every_local_mode_rejects_atomically(routine_prelude,
                                                "LCEP-COMP-003F parameterized routine declaration and call source");
}

}  // namespace

int main() {
    if (!rejects_each_remaining_quantum_and_control_category()) return 1;
    std::cout << "SynQ local classical evaluation full matrix smoke test passed\n";
    return 0;
}
