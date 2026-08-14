#include <iostream>
#include <string>

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

bool parses_lowers_and_resolves_named_measurement_results() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "measure q[0] as observed\n"
        "if observed then quantum h q[0]\n");
    if (!require(parsed.ok(), "named top-level measurement result and later Boolean condition parse")) return false;

    const auto* measurement = dynamic_cast<const MeasurementNode*>(parsed.program->statements.at(0));
    if (!require(measurement != nullptr && measurement->result_name.has_value() &&
                     *measurement->result_name == "observed",
                 "typed measurement node preserves the declared result name")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "named measurement-result fixture lowers to Hybrid IR")) return false;
    const auto* hybrid_measurement = std::get_if<synq::compiler::HybridMeasurement>(&lowered.program->nodes.at(0));
    if (!require(hybrid_measurement != nullptr && hybrid_measurement->result_name.has_value() &&
                     *hybrid_measurement->result_name == "observed",
                 "Hybrid IR preserves named measurement-result provenance")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "measurement result is a static Boolean binding for later control validation")) return false;
    const auto* control = std::get_if<synq::compiler::ResolvedHybridControlFlow>(&resolved.program->nodes.at(1));
    return require(control != nullptr && control->condition_binding_index.has_value() &&
                       *control->condition_binding_index == 0,
                   "resolver records the earlier measurement result binding index without a value");
}

bool rejects_malformed_duplicate_and_forward_result_uses() {
    Parser parser;
    const auto malformed = parser.parseSourceWithDiagnostics("measure q[0] as 9observed\n");
    if (!require(!malformed.ok() && has_code(malformed.diagnostics, "SYNQ-P008"),
                 "measurement result requires a valid identifier")) return false;

    const auto duplicate = parser.parseSourceWithDiagnostics("let observed = true\nmeasure q[0] as observed\n");
    if (!require(!duplicate.ok() && has_code(duplicate.diagnostics, "SYNQ-S004"),
                 "measurement result shares top-level declaration uniqueness")) return false;

    const auto forward = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "if observed then quantum h q[0]\n"
        "measure q[0] as observed\n");
    if (!require(forward.ok(), "forward measurement-result fixture remains a successful parse")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*forward.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    return require(!resolved.ok() && has_code(resolved.diagnostics, "SYNQ-R002"),
                   "forward measurement result is rejected by bounded Boolean resolution");
}

bool preserves_legacy_measurement_and_rejects_unsupported_export_lowering() {
    Parser parser;
    const auto legacy = parser.parseSourceWithDiagnostics("measure q[0]\n");
    if (!require(legacy.ok(), "legacy measurement syntax remains accepted")) return false;
    const auto* measurement = dynamic_cast<const MeasurementNode*>(legacy.program->statements.front());
    if (!require(measurement != nullptr && !measurement->result_name.has_value(),
                 "legacy measurement has no invented result name")) return false;

    const auto named = parser.parseSourceWithDiagnostics("measure q[0] as observed\n");
    if (!require(named.ok(), "named measurement parses for exporter rejection fixture")) return false;
    const auto exported = synq::compiler::export_openqasm3(*named.program);
    return require(!exported.ok(),
                   "OpenQASM exporter rejects named measurement results rather than silently dropping the binding");
}

}  // namespace

int main() {
    if (!parses_lowers_and_resolves_named_measurement_results()) return 1;
    if (!rejects_malformed_duplicate_and_forward_result_uses()) return 1;
    if (!preserves_legacy_measurement_and_rejects_unsupported_export_lowering()) return 1;
    std::cout << "SynQ typed measurement-result smoke test passed\n";
    return 0;
}
