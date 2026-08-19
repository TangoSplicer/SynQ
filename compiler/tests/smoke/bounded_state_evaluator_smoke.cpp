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

synq::compiler::BoundedStateEvaluationResult evaluate(const synq::compiler::ResolvedHybridProgram& program,
                                                       std::size_t max_cells = 64,
                                                       std::size_t max_transitions = 128,
                                                       std::size_t max_depth = 16,
                                                       std::size_t max_operations = 128) {
    synq::compiler::BoundedStateEvaluationOptions options;
    options.allow_experimental_state_evaluation = true;
    options.max_state_cells = max_cells;
    options.max_state_transitions = max_transitions;
    options.max_expression_depth = max_depth;
    options.max_operations = max_operations;
    return synq::compiler::evaluate_bounded_state(program, options);
}

bool evaluates_typed_cells_in_source_order() {
    synq::compiler::ResolvedHybridProgram program;
    synq::compiler::HybridProgram lowered_program;
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
    if (!require(prepare(source, program, &lowered_program), "supported U2 fixture parses, lowers, and resolves")) return false;
    if (!require(lowered_program.nodes.size() == 7 &&
                     std::holds_alternative<synq::compiler::HybridMutableDeclaration>(lowered_program.nodes[1]) &&
                     std::holds_alternative<synq::compiler::HybridAssignment>(lowered_program.nodes[2]) &&
                     std::get<synq::compiler::HybridMutableDeclaration>(lowered_program.nodes[1]).name == "count" &&
                     std::get<synq::compiler::HybridAssignment>(lowered_program.nodes[2]).target_name == "count",
                 "parser lowering preserves typed mutable declaration and assignment provenance")) return false;
    const auto result = evaluate(program);
    if (!require(result.ok() && result.evaluation->cells.size() == 3,
                 "state evaluation produces three final cells")) return false;
    const auto& cells = result.evaluation->cells;
    return require(cells[0].name == "count" && cells[0].value.integer_value == 7 &&
                       cells[0].declaration_span.line == 5 && cells[0].last_write_span.line == 6 &&
                       cells[1].name == "armed" && !cells[1].value.boolean_value &&
                       cells[2].name == "title" && cells[2].value.string_value == "ready",
                   "same-cell reads use the pre-write snapshot and retain final source provenance");
}

bool enforces_parser_and_resolver_boundaries() {
    Parser parser;
    const auto gate_missing = parser.parseSourceWithDiagnostics("var ready = true\n");
    if (!require(!gate_missing.ok() && has_code(gate_missing.diagnostics, "SYNQ-P014"),
                 "mutable syntax requires its Alpha feature gate")) return false;
    Parser malformed_parser;
    const auto malformed = malformed_parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"mutable-classical-state\")]\nset = true\n");
    if (!require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P016"),
                 "malformed whole-cell assignment has a dedicated parse diagnostic")) return false;

    const auto immutable_target = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"mutable-classical-state\")]\nlet fixed = true\nset fixed = false\n");
    if (!require(immutable_target.ok(), "immutable-target rejection fixture parses")) return false;
    const auto immutable_lowered = synq::compiler::lower_to_hybrid_ir(*immutable_target.program);
    const auto immutable_resolved = synq::compiler::resolve_hybrid_names(*immutable_lowered.program);
    if (!require(!immutable_resolved.ok() && has_code(immutable_resolved.diagnostics, "SYNQ-S006"),
                 "set rejects immutable declaration targets")) return false;

    const auto mismatch = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"mutable-classical-state\")]\nvar counter = 1\nset counter = true\n");
    if (!require(mismatch.ok(), "type-mismatch fixture parses")) return false;
    const auto mismatch_lowered = synq::compiler::lower_to_hybrid_ir(*mismatch.program);
    const auto mismatch_resolved = synq::compiler::resolve_hybrid_names(*mismatch_lowered.program);
    if (!require(!mismatch_resolved.ok() && has_code(mismatch_resolved.diagnostics, "SYNQ-S007"),
                 "set requires the cell's exact existing static type")) return false;

    const auto immutable_read = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"mutable-classical-state\")]\nvar state = true\nlet snapshot = state\n");
    if (!require(immutable_read.ok(), "immutable-read fixture parses")) return false;
    const auto immutable_read_lowered = synq::compiler::lower_to_hybrid_ir(*immutable_read.program);
    const auto immutable_read_resolved = synq::compiler::resolve_hybrid_names(*immutable_read_lowered.program);
    return require(!immutable_read_resolved.ok() && has_code(immutable_read_resolved.diagnostics, "SYNQ-S005"),
                   "let declarations remain independent from mutable state");
}

bool enforces_opt_in_and_resource_bounds_atomically() {
    synq::compiler::ResolvedHybridProgram program;
    if (!require(prepare(
                     "#[experimental(feature = \"mutable-classical-state\")]\n"
                     "#[experimental(feature = \"classical-control-flow\")]\n"
                     "var armed = true\nset armed = not armed\n",
                     program),
                 "state-bound fixture parses, lowers, and resolves")) return false;
    synq::compiler::BoundedStateEvaluationOptions disabled;
    const auto disabled_result = synq::compiler::evaluate_bounded_state(program, disabled);
    if (!require(!disabled_result.ok() && !disabled_result.evaluation.has_value() &&
                     has_code(disabled_result.diagnostics, "SYNQ-E008"),
                 "state evaluation requires explicit API opt-in")) return false;
    const auto transition_result = evaluate(program, 64, 1);
    if (!require(!transition_result.ok() && !transition_result.evaluation.has_value() &&
                     has_code(transition_result.diagnostics, "SYNQ-E009"),
                 "state-transition exhaustion returns no partial state result")) return false;
    const auto operation_result = evaluate(program, 64, 128, 16, 0);
    if (!require(!operation_result.ok() && !operation_result.evaluation.has_value() &&
                     has_code(operation_result.diagnostics, "SYNQ-E007"),
                 "state evaluation reuses the explicit Boolean operation budget")) return false;

    synq::compiler::ResolvedHybridProgram cells_program;
    if (!require(prepare(
                     "#[experimental(feature = \"mutable-classical-state\")]\n"
                     "var first = 1\nvar second = 2\n",
                     cells_program),
                 "cell-limit fixture parses, lowers, and resolves")) return false;
    const auto cells_result = evaluate(cells_program, 1);
    return require(!cells_result.ok() && !cells_result.evaluation.has_value() &&
                       has_code(cells_result.diagnostics, "SYNQ-E009"),
                   "state-cell exhaustion returns no partial state result");
}

bool rejects_state_in_quantum_paths() {
    synq::compiler::ResolvedHybridProgram resolved_program;
    synq::compiler::HybridProgram lowered_program;
    const std::string source =
        "#[experimental(feature = \"mutable-classical-state\")]\n"
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "var ready = true\nqubit q[1]\nquantum h q[0]\n";
    if (!require(prepare(source, resolved_program, &lowered_program),
                 "state plus quantum rejection fixture parses, lowers, and resolves")) return false;
    const auto state_result = evaluate(resolved_program);
    if (!require(!state_result.ok() && !state_result.evaluation.has_value() &&
                     has_code(state_result.diagnostics, "SYNQ-E010"),
                 "state evaluator rejects quantum nodes instead of executing a hybrid program")) return false;
    const auto exported = synq::compiler::export_hybrid_openqasm3(lowered_program);
    if (!require(!exported.ok() && !exported.diagnostics.empty() &&
                     exported.diagnostics.front().find("mutable SynQ state") != std::string::npos,
                 "strict Hybrid export rejects mutable state instead of inventing target storage")) return false;
    synq::compiler::BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulated = synq::compiler::simulate_bounded_quantum(resolved_program, options);
    return require(!simulated.ok() && has_code(simulated.diagnostics, "SYNQ-SIM002"),
                   "quantum simulator rejects mutable state rather than executing it");
}

}  // namespace

int main() {
    if (!evaluates_typed_cells_in_source_order()) return 1;
    if (!enforces_parser_and_resolver_boundaries()) return 1;
    if (!enforces_opt_in_and_resource_bounds_atomically()) return 1;
    if (!rejects_state_in_quantum_paths()) return 1;
    std::cout << "SynQ bounded mutable-state evaluator smoke test passed\n";
    return 0;
}
