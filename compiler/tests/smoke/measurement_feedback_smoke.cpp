#include <cmath>
#include <iostream>
#include <string>

#include "compiler/bounded_simulator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

namespace {

using synq::compiler::BoundedSimulationOptions;
using synq::compiler::Diagnostic;

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool has_code(const std::vector<Diagnostic>& diagnostics, const std::string& code) {
    return diagnostics.size() == 1 && diagnostics.front().code == code;
}

const char* u4_prelude() {
    return
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "#[experimental(feature = \"measurement-feedback\")]\n";
}

bool parses_lowers_resolves_and_exports_one_feedback_pair() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(std::string(u4_prelude()) +
        "qubit q[2]\n"
        "quantum h q[0]\n"
        "measure q[0] as observed\n"
        "if observed then quantum x q[1]\n");
    if (!require(parsed.ok(), "U4 named measurement and immediate direct X correction parse")) return false;

    const auto* measurement = dynamic_cast<const MeasurementNode*>(parsed.program->statements.at(2));
    const auto* control = dynamic_cast<const ClassicalControlNode*>(parsed.program->statements.at(3));
    if (!require(measurement != nullptr && measurement->feedback_enabled && measurement->result_name == "observed" &&
                     control != nullptr && control->feedback_enabled,
                 "typed AST preserves U4 opt-in provenance on the named measurement and correction")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "U4 feedback fixture lowers to Hybrid IR")) return false;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "resolver classifies the direct U4 measurement feedback pair")) return false;
    const auto* feedback = std::get_if<synq::compiler::ResolvedHybridMeasurementFeedback>(&resolved.program->nodes.at(2));
    if (!require(feedback != nullptr && feedback->measurement.result_name == "observed" &&
                     feedback->correction.kind == ClassicalControlKind::If,
                 "resolved program retains one atomic feedback pair with named-result provenance")) return false;

    const auto exported = synq::compiler::export_hybrid_openqasm3(*lowered.program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[2] q;\n"
        "bit synq_measure_observed;\n"
        "h q[0];\n"
        "synq_measure_observed = measure q[0];\n"
        "if (synq_measure_observed) x q[1];\n";
    return require(exported.ok() && exported.program == expected,
                   "strict Hybrid export lowers the U4 pair to one scalar bit, one measurement assignment, and one conditional X gate");
}

bool enforces_feature_gate_and_resolver_boundaries() {
    Parser parser;
    const auto missing_gate = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[2]\n"
        "measure q[0] as observed\n"
        "if observed then quantum x q[1]\n");
    if (!require(!missing_gate.ok() && has_code(missing_gate.diagnostics, "SYNQ-P007"),
                 "exact named-measurement/direct-X feedback requires the dedicated U4 feature gate")) return false;

    const auto check = [&parser](const std::string& source, const std::string& code, const std::string& message) {
        const auto parsed = parser.parseSourceWithDiagnostics(std::string(u4_prelude()) + source);
        if (!require(parsed.ok(), message + " parses before resolver validation")) return false;
        const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
        if (!require(lowered.ok(), message + " lowers before resolver validation")) return false;
        const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
        return require(!resolved.ok() && has_code(resolved.diagnostics, code), message);
    };

    if (!check("qubit q[2]\nmeasure q[0] as observed\nif not observed then quantum x q[1]\n",
               "SYNQ-R007", "negated measurement results are rejected")) return false;
    if (!check("qubit q[2]\nmeasure q[0] as observed\nif observed then quantum h q[1]\n",
               "SYNQ-R008", "non-X feedback corrections are rejected")) return false;
    if (!check("qubit q[2]\nmeasure q[0] as observed\nquantum h q[1]\nif observed then quantum x q[1]\n",
               "SYNQ-R007", "non-adjacent measurement result uses are rejected")) return false;
    if (!check("qubit q[2]\nmeasure q[0] as observed\nif observed then quantum x q[2]\n",
               "SYNQ-Q002", "out-of-range feedback correction operands are rejected")) return false;
    return check("qubit q[2]\nmeasure q[0] as observed\nif observed then quantum x q[1]\nquantum h q[0]\n",
                 "SYNQ-R007", "the one U4 feedback pair is terminal in the bounded envelope");
}

bool simulates_branch_enumerated_feedback_without_sampling() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(std::string(u4_prelude()) +
        "qubit q[2]\n"
        "quantum h q[0]\n"
        "measure q[0] as observed\n"
        "if observed then quantum x q[1]\n");
    if (!require(parsed.ok(), "branch-enumeration fixture parses")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "branch-enumeration fixture resolves")) return false;
    BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulated = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
    if (!require(simulated.ok() && simulated.simulation->basis_probabilities.size() == 2 &&
                     simulated.simulation->measurements.size() == 1,
                 "U4 local model returns weighted final probabilities and the unsampled measurement marginal")) return false;
    const auto& probabilities = simulated.simulation->basis_probabilities;
    if (!require(probabilities.at(0).basis_index == 0 && probabilities.at(1).basis_index == 3 &&
                     std::abs(probabilities.at(0).probability - 0.5) < 1e-9 &&
                     std::abs(probabilities.at(1).probability - 0.5) < 1e-9 &&
                     std::abs(simulated.simulation->measurements.front().probability_one - 0.5) < 1e-9,
                 "one branch retains |00>, the observed-one branch applies X to produce |11>, and no sampled bit is exposed")) return false;

    const auto reset_parsed = parser.parseSourceWithDiagnostics(std::string(u4_prelude()) +
        "qubit q[1]\n"
        "quantum h q[0]\n"
        "measure q[0] as observed\n"
        "if observed then quantum x q[0]\n");
    const auto reset_lowered = synq::compiler::lower_to_hybrid_ir(*reset_parsed.program);
    const auto reset_resolved = synq::compiler::resolve_hybrid_names(*reset_lowered.program);
    const auto reset_simulated = synq::compiler::simulate_bounded_quantum(*reset_resolved.program, options);
    return require(reset_parsed.ok() && reset_resolved.ok() && reset_simulated.ok() &&
                       reset_simulated.simulation->basis_probabilities.size() == 1 &&
                       reset_simulated.simulation->basis_probabilities.front().basis_index == 0 &&
                       std::abs(reset_simulated.simulation->basis_probabilities.front().probability - 1.0) < 1e-9,
                   "same-qubit conditional X correction deterministically restores the ideal |0> branch mixture without sampling");
}

bool preserves_legacy_named_result_boundary_without_u4() {
    Parser parser;
    const auto legacy = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "measure q[0] as observed\n"
        "if observed then quantum h q[0]\n");
    if (!require(legacy.ok(), "legacy named-result static Boolean provenance remains parseable without U4")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*legacy.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "legacy named-result static provenance remains resolvable")) return false;
    const auto exported = synq::compiler::export_hybrid_openqasm3(*lowered.program);
    BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulated = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
    return require(!exported.ok() && !simulated.ok() && has_code(simulated.diagnostics, "SYNQ-SIM002"),
                   "non-U4 named result controls retain explicit strict-export and simulator rejection boundaries");
}

}  // namespace

int main() {
    if (!parses_lowers_resolves_and_exports_one_feedback_pair()) return 1;
    if (!enforces_feature_gate_and_resolver_boundaries()) return 1;
    if (!simulates_branch_enumerated_feedback_without_sampling()) return 1;
    if (!preserves_legacy_named_result_boundary_without_u4()) return 1;
    std::cout << "SynQ measurement feedback smoke test passed\n";
    return 0;
}
