#include "bounded_simulator.h"

#include <cmath>
#include <complex>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace synq::compiler {
namespace {

using Complex = std::complex<double>;
constexpr double kPi = 3.141592653589793238462643383279502884;
constexpr double kProbabilityEpsilon = 1e-12;

struct RegisterAllocation {
    std::size_t physical_offset = 0;
    std::size_t qubit_count = 0;
};

Diagnostic error(const std::string& code, const SourceSpan& span, std::string message, std::string help) {
    return Diagnostic{code, DiagnosticSeverity::Error, span, std::move(message), std::move(help)};
}

bool parse_angle(const std::string& text, double& angle) {
    if (text == "pi") { angle = kPi; return true; }
    if (text == "-pi") { angle = -kPi; return true; }
    const bool negative = text.rfind("-pi/", 0) == 0;
    const bool positive = text.rfind("pi/", 0) == 0;
    if (negative || positive) {
        const std::string divisor_text = text.substr(negative ? 4 : 3);
        try {
            const std::size_t divisor = static_cast<std::size_t>(std::stoull(divisor_text));
            if (divisor == 0) return false;
            angle = (negative ? -kPi : kPi) / static_cast<double>(divisor);
            return true;
        } catch (...) {
            return false;
        }
    }
    try {
        std::size_t consumed = 0;
        angle = std::stod(text, &consumed);
        return consumed == text.size() && std::isfinite(angle);
    } catch (...) {
        return false;
    }
}

void apply_single(std::vector<Complex>& state, std::size_t qubit, Complex a, Complex b, Complex c, Complex d) {
    const std::size_t mask = std::size_t{1} << qubit;
    for (std::size_t basis = 0; basis < state.size(); ++basis) {
        if ((basis & mask) != 0) continue;
        const std::size_t paired = basis | mask;
        const Complex zero = state[basis];
        const Complex one = state[paired];
        state[basis] = a * zero + b * one;
        state[paired] = c * zero + d * one;
    }
}

void apply_cx(std::vector<Complex>& state, std::size_t control, std::size_t target) {
    const std::size_t control_mask = std::size_t{1} << control;
    const std::size_t target_mask = std::size_t{1} << target;
    for (std::size_t basis = 0; basis < state.size(); ++basis) {
        if ((basis & control_mask) == 0 || (basis & target_mask) != 0) continue;
        const std::size_t paired = basis | target_mask;
        std::swap(state[basis], state[paired]);
    }
}

bool apply_gate(const HybridQuantumGate& gate, std::vector<Complex>& state, Diagnostic& diagnostic) {
    const auto require_one = [&]() -> bool {
        if (gate.qubit_indices.size() == 1) return true;
        diagnostic = error("SYNQ-SIM003", gate.span, "simulator received an unsupported single-qubit gate shape",
                           "use a parser-produced supported gate with one in-range operand");
        return false;
    };
    const auto require_two = [&]() -> bool {
        if (gate.qubit_indices.size() == 2) return true;
        diagnostic = error("SYNQ-SIM003", gate.span, "simulator received an unsupported two-qubit gate shape",
                           "use a parser-produced supported gate with two distinct in-range operands");
        return false;
    };
    const Complex i{0.0, 1.0};
    const double inverse_sqrt_two = 1.0 / std::sqrt(2.0);

    switch (gate.kind) {
        case QuantumGateKind::H:
            if (!require_one()) return false;
            apply_single(state, gate.qubit_indices[0], inverse_sqrt_two, inverse_sqrt_two,
                         inverse_sqrt_two, -inverse_sqrt_two);
            return true;
        case QuantumGateKind::X:
            if (!require_one()) return false;
            apply_single(state, gate.qubit_indices[0], 0.0, 1.0, 1.0, 0.0);
            return true;
        case QuantumGateKind::Y:
            if (!require_one()) return false;
            apply_single(state, gate.qubit_indices[0], 0.0, -i, i, 0.0);
            return true;
        case QuantumGateKind::Z:
            if (!require_one()) return false;
            apply_single(state, gate.qubit_indices[0], 1.0, 0.0, 0.0, -1.0);
            return true;
        case QuantumGateKind::Cx:
            if (!require_two() || gate.qubit_indices[0] == gate.qubit_indices[1]) {
                if (diagnostic.code.empty()) diagnostic = error("SYNQ-SIM003", gate.span, "controlled-X requires distinct operands",
                                                                "use two distinct declared qubit indices");
                return false;
            }
            apply_cx(state, gate.qubit_indices[0], gate.qubit_indices[1]);
            return true;
        case QuantumGateKind::BellPair:
            if (!require_two() || gate.qubit_indices[0] == gate.qubit_indices[1]) {
                if (diagnostic.code.empty()) diagnostic = error("SYNQ-SIM003", gate.span, "bell_pair requires distinct operands",
                                                                "use two distinct declared qubit indices");
                return false;
            }
            apply_single(state, gate.qubit_indices[0], inverse_sqrt_two, inverse_sqrt_two,
                         inverse_sqrt_two, -inverse_sqrt_two);
            apply_cx(state, gate.qubit_indices[0], gate.qubit_indices[1]);
            return true;
        case QuantumGateKind::Rx:
        case QuantumGateKind::Ry:
        case QuantumGateKind::Rz:
        case QuantumGateKind::Phase: {
            if (!require_one() || !gate.literal_angle.has_value()) {
                if (diagnostic.code.empty()) diagnostic = error("SYNQ-SIM003", gate.span, "parameterized simulation gate requires one literal angle and one operand",
                                                                "use a parser-produced supported parameterized gate");
                return false;
            }
            double angle = 0.0;
            if (!parse_angle(*gate.literal_angle, angle)) {
                diagnostic = error("SYNQ-SIM003", gate.span, "simulator cannot interpret the literal gate angle",
                                   "use a documented decimal or pi-form literal angle");
                return false;
            }
            if (gate.kind == QuantumGateKind::Rx) {
                const double half = angle / 2.0;
                apply_single(state, gate.qubit_indices[0], std::cos(half), -i * std::sin(half),
                             -i * std::sin(half), std::cos(half));
            } else if (gate.kind == QuantumGateKind::Ry) {
                const double half = angle / 2.0;
                apply_single(state, gate.qubit_indices[0], std::cos(half), -std::sin(half),
                             std::sin(half), std::cos(half));
            } else if (gate.kind == QuantumGateKind::Rz) {
                const double half = angle / 2.0;
                apply_single(state, gate.qubit_indices[0], std::exp(-i * half), 0.0,
                             0.0, std::exp(i * half));
            } else {
                apply_single(state, gate.qubit_indices[0], 1.0, 0.0, 0.0, std::exp(i * angle));
            }
            return true;
        }
        case QuantumGateKind::Unknown:
            diagnostic = error("SYNQ-SIM003", gate.span, "simulator does not support this quantum gate",
                               "use h, x, y, z, cx, bell_pair, rx, ry, rz, or p");
            return false;
    }
    diagnostic = error("SYNQ-SIM003", gate.span, "simulator received an unknown gate kind",
                       "use a parser-produced supported gate");
    return false;
}

double probability_one(const std::vector<Complex>& state, std::size_t qubit) {
    const std::size_t mask = std::size_t{1} << qubit;
    double probability = 0.0;
    for (std::size_t basis = 0; basis < state.size(); ++basis) {
        if ((basis & mask) != 0) probability += std::norm(state[basis]);
    }
    return probability;
}

}  // namespace

bool BoundedSimulationResult::ok() const { return simulation.has_value() && diagnostics.empty(); }

BoundedSimulationResult simulate_bounded_quantum(const ResolvedHybridProgram& program,
                                                  const BoundedSimulationOptions& options) {
    BoundedSimulationResult result;
    if (!options.allow_experimental_local_simulation) {
        result.diagnostics.push_back(error("SYNQ-SIM000", {}, "bounded local simulation requires explicit opt-in",
                                           "set allow_experimental_local_simulation to true after reviewing the limits"));
        return result;
    }

    std::size_t qubit_count = 0;
    std::vector<SimulatedRegister> registers;
    std::unordered_map<std::string, RegisterAllocation> allocations;
    std::vector<HybridQuantumGate> gates;
    std::vector<HybridMeasurement> measurements;
    bool measurements_started = false;
    for (const auto& node : program.nodes) {
        if (const auto* qubits = std::get_if<HybridQubitDeclaration>(&node)) {
            if (allocations.find(qubits->name) != allocations.end()) {
                result.diagnostics.push_back(error("SYNQ-SIM001", qubits->span,
                                                   "simulator received a duplicate explicit qubit register declaration",
                                                   "use unique parser-produced qubit register declarations"));
                return result;
            }
            if (qubits->qubit_count == 0 || qubits->qubit_count > options.max_qubits ||
                qubit_count > options.max_qubits - qubits->qubit_count) {
                result.diagnostics.push_back(error("SYNQ-SIM001", qubits->span,
                                                   "simulator register declarations exceed the configured qubit limit",
                                                   "declare a positive combined register size no larger than max_qubits"));
                return result;
            }
            allocations.emplace(qubits->name, RegisterAllocation{qubit_count, qubits->qubit_count});
            registers.push_back({qubits->name, qubits->qubit_count, qubit_count});
            qubit_count += qubits->qubit_count;
            continue;
        }
        if (const auto* gate = std::get_if<HybridQuantumGate>(&node)) {
            if (measurements_started) {
                result.diagnostics.push_back(error("SYNQ-SIM002", gate->span,
                                                   "simulator accepts measurements only after all quantum gates",
                                                   "move measurements to the end of the bounded simulation program"));
                return result;
            }
            if (gate->qubit_register_names.size() != gate->qubit_indices.size()) {
                result.diagnostics.push_back(error("SYNQ-SIM003", gate->span,
                                                   "simulator received mismatched quantum operand metadata",
                                                   "use a parser-produced supported gate with declared register operands"));
                return result;
            }
            HybridQuantumGate rebased = *gate;
            for (std::size_t position = 0; position < rebased.qubit_indices.size(); ++position) {
                const auto allocation = allocations.find(rebased.qubit_register_names[position]);
                if (allocation == allocations.end() ||
                    rebased.qubit_indices[position] >= allocation->second.qubit_count) {
                    result.diagnostics.push_back(error("SYNQ-SIM001", gate->span,
                                                       "simulator cannot map a gate operand to an explicit qubit register",
                                                       "declare the referenced register before simulation and use an in-range index"));
                    return result;
                }
                rebased.qubit_indices[position] = allocation->second.physical_offset + rebased.qubit_indices[position];
            }
            gates.push_back(std::move(rebased));
            continue;
        }
        if (const auto* measurement = std::get_if<HybridMeasurement>(&node)) {
            if (measurement->result_name.has_value()) {
                result.diagnostics.push_back(error("SYNQ-SIM002", measurement->span,
                                                   "simulator does not sample or bind named measurement results",
                                                   "use an unnamed trailing measurement to obtain a marginal probability"));
                return result;
            }
            measurements_started = true;
            const auto allocation = allocations.find(measurement->qubit_register_name);
            if (allocation == allocations.end() || measurement->qubit_index >= allocation->second.qubit_count) {
                result.diagnostics.push_back(error("SYNQ-SIM001", measurement->span,
                                                   "simulator cannot map a measurement operand to an explicit qubit register",
                                                   "declare the referenced register before simulation and use an in-range index"));
                return result;
            }
            HybridMeasurement rebased = *measurement;
            rebased.qubit_index = allocation->second.physical_offset + measurement->qubit_index;
            measurements.push_back(std::move(rebased));
            continue;
        }
        result.diagnostics.push_back(error("SYNQ-SIM002", {},
                                           "bounded local simulation rejects this typed program construct",
                                           "use only qubit q[n], supported gates, and trailing unnamed measurements"));
        return result;
    }

    if (registers.empty() || qubit_count == 0 || qubit_count > options.max_qubits) {
        result.diagnostics.push_back(error("SYNQ-SIM001", {},
                                           "simulator requires explicit qubit register declarations inside the configured qubit limit",
                                           "declare one or more `qubit name[n]` registers totaling 1 through max_qubits qubits before simulation"));
        return result;
    }
    if (gates.size() > options.max_operations) {
        result.diagnostics.push_back(error("SYNQ-SIM004", {}, "simulator exceeds the configured gate-operation limit",
                                           "reduce the circuit or explicitly choose a larger documented limit"));
        return result;
    }

    std::vector<Complex> state(std::size_t{1} << qubit_count, Complex{0.0, 0.0});
    state.front() = Complex{1.0, 0.0};
    for (const auto& gate : gates) {
        Diagnostic diagnostic;
        if (!apply_gate(gate, state, diagnostic)) {
            result.diagnostics.push_back(std::move(diagnostic));
            return result;
        }
    }

    double norm = 0.0;
    for (const auto& amplitude : state) norm += std::norm(amplitude);
    if (!std::isfinite(norm) || std::abs(norm - 1.0) > 1e-9) {
        result.diagnostics.push_back(error("SYNQ-SIM005", {}, "simulator state normalization check failed",
                                           "reduce the circuit and report the reproducible input; no result was produced"));
        return result;
    }

    BoundedSimulation simulation;
    simulation.qubit_count = qubit_count;
    simulation.registers = registers;
    for (std::size_t basis = 0; basis < state.size(); ++basis) {
        const double probability = std::norm(state[basis]);
        if (probability > kProbabilityEpsilon) simulation.basis_probabilities.push_back({basis, probability});
    }
    for (const auto& measurement : measurements) {
        const auto allocation = allocations.find(measurement.qubit_register_name);
        const std::size_t source_index = measurement.qubit_index - allocation->second.physical_offset;
        simulation.measurements.push_back({measurement.qubit_register_name, source_index, measurement.qubit_index,
                                           probability_one(state, measurement.qubit_index)});
    }
    result.simulation = std::move(simulation);
    return result;
}

}  // namespace synq::compiler
