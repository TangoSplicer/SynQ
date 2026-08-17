#include <cmath>
#include <iostream>
#include <string>

#include "compiler/bounded_simulator.h"
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

bool near(double actual, double expected) { return std::abs(actual - expected) < 1e-9; }

bool has_code(const std::vector<synq::compiler::Diagnostic>& diagnostics, const std::string& code) {
    return diagnostics.size() == 1 && diagnostics.front().code == code;
}

bool simulate_source(const std::string& source, synq::compiler::BoundedSimulationResult& result,
                     std::size_t max_qubits = 10) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return false;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return false;
    synq::compiler::BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    options.max_qubits = max_qubits;
    result = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
    return true;
}

bool simulates_bell_and_parameterized_states() {
    synq::compiler::BoundedSimulationResult bell;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\n"
                     "qubit q[2]\nquantum bell_pair q[0], q[1]\nmeasure q[0]\nmeasure q[1]\n", bell),
                 "Bell fixture parses, lowers, and resolves")) return false;
    if (!require(bell.ok() && bell.simulation->basis_probabilities.size() == 2 &&
                     bell.simulation->basis_probabilities[0].basis_index == 0 &&
                     near(bell.simulation->basis_probabilities[0].probability, 0.5) &&
                     bell.simulation->basis_probabilities[1].basis_index == 3 &&
                     near(bell.simulation->basis_probabilities[1].probability, 0.5) &&
                     bell.simulation->measurements.size() == 2 &&
                     near(bell.simulation->measurements[0].probability_one, 0.5) &&
                     near(bell.simulation->measurements[1].probability_one, 0.5),
                 "Bell simulation returns deterministic basis and marginal probabilities")) return false;

    synq::compiler::BoundedSimulationResult rotation;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\n"
                     "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
                     "qubit q[1]\nquantum rx(pi/2) q[0]\nmeasure q[0]\n", rotation),
                 "rotation fixture parses, lowers, and resolves")) return false;
    return require(rotation.ok() && rotation.simulation->basis_probabilities.size() == 2 &&
                       near(rotation.simulation->measurements[0].probability_one, 0.5),
                   "parameterized rotation simulation retains normalized probability" );
}

bool enforces_opt_in_and_resource_or_semantic_boundaries() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\nqubit q[1]\nquantum h q[0]\n");
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    synq::compiler::BoundedSimulationOptions disabled;
    const auto disabled_result = synq::compiler::simulate_bounded_quantum(*resolved.program, disabled);
    if (!require(!disabled_result.ok() && has_code(disabled_result.diagnostics, "SYNQ-SIM000"),
                 "simulation requires explicit API opt-in")) return false;

    synq::compiler::BoundedSimulationResult large;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\nqubit q[11]\n", large),
                 "large-register fixture parses, lowers, and resolves")) return false;
    if (!require(!large.ok() && has_code(large.diagnostics, "SYNQ-SIM001"),
                 "simulation enforces the qubit resource limit")) return false;

    synq::compiler::BoundedSimulationResult named_measurement;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\n"
                     "qubit q[1]\nmeasure q[0] as observed\n", named_measurement),
                 "named-measurement fixture parses, lowers, and resolves")) return false;
    if (!require(!named_measurement.ok() && has_code(named_measurement.diagnostics, "SYNQ-SIM002"),
                   "simulator rejects sampling/binding semantics it does not implement")) return false;

    synq::compiler::BoundedSimulationResult named_registers;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\n"
                     "#[experimental(feature = \"named-qubit-register-operands\")]\n"
                     "qubit data[1]\n"
                     "qubit ancilla[1]\n"
                     "quantum bell_pair data[0], ancilla[0]\n"
                     "measure data[0]\nmeasure ancilla[0]\n", named_registers),
                 "multi-register fixture parses, lowers, and resolves")) return false;
    if (!require(named_registers.ok() && named_registers.simulation->qubit_count == 2 &&
                     named_registers.simulation->registers.size() == 2 &&
                     named_registers.simulation->registers[0].name == "data" &&
                     named_registers.simulation->registers[0].physical_offset == 0 &&
                     named_registers.simulation->registers[1].name == "ancilla" &&
                     named_registers.simulation->registers[1].physical_offset == 1 &&
                     named_registers.simulation->basis_probabilities.size() == 2 &&
                     named_registers.simulation->basis_probabilities[1].basis_index == 3 &&
                     named_registers.simulation->measurements.size() == 2 &&
                     named_registers.simulation->measurements[0].register_name == "data" &&
                     named_registers.simulation->measurements[0].register_index == 0 &&
                     named_registers.simulation->measurements[0].qubit_index == 0 &&
                     named_registers.simulation->measurements[1].register_name == "ancilla" &&
                     named_registers.simulation->measurements[1].register_index == 0 &&
                     named_registers.simulation->measurements[1].qubit_index == 1 &&
                     near(named_registers.simulation->measurements[0].probability_one, 0.5) &&
                     near(named_registers.simulation->measurements[1].probability_one, 0.5),
                 "multi-register simulation preserves source identity and declaration-order physical allocation")) return false;

    synq::compiler::BoundedSimulationResult combined_limit;
    if (!require(simulate_source(
                     "#[experimental(feature = \"qubit-declarations\")]\n"
                     "qubit q[6]\nqubit ancilla[5]\n", combined_limit),
                 "combined-register resource-limit fixture parses, lowers, and resolves")) return false;
    return require(!combined_limit.ok() && has_code(combined_limit.diagnostics, "SYNQ-SIM001"),
                   "multi-register simulation enforces the combined configured qubit limit");
}

}  // namespace

int main() {
    if (!simulates_bell_and_parameterized_states()) return 1;
    if (!enforces_opt_in_and_resource_or_semantic_boundaries()) return 1;
    std::cout << "SynQ bounded simulator smoke test passed\n";
    return 0;
}
