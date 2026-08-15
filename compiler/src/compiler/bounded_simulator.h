// Deterministic probability simulation for a small, explicitly opted-in typed
// quantum subset. This is not a provider adapter or general quantum runtime.
#ifndef SYNQ_COMPILER_BOUNDED_SIMULATOR_H
#define SYNQ_COMPILER_BOUNDED_SIMULATOR_H

#include <cstddef>
#include <optional>
#include <vector>

#include "diagnostic.h"
#include "name_resolution.h"

namespace synq::compiler {

struct BasisProbability {
    std::size_t basis_index = 0;
    double probability = 0.0;
};

struct MeasurementProbability {
    std::size_t qubit_index = 0;
    double probability_one = 0.0;
};

struct BoundedSimulation {
    std::size_t qubit_count = 0;
    std::vector<BasisProbability> basis_probabilities;
    std::vector<MeasurementProbability> measurements;
};

struct BoundedSimulationOptions {
    bool allow_experimental_local_simulation = false;
    std::size_t max_qubits = 10;
    std::size_t max_operations = 1024;
};

struct BoundedSimulationResult {
    std::optional<BoundedSimulation> simulation;
    std::vector<Diagnostic> diagnostics;

    bool ok() const;
};

// Simulates a pure state prepared from |0...0> by an explicit `qubit q[n]`
// declaration, supported gates, and optional trailing unnamed measurements.
// Measurements report marginal probabilities and do not sample or collapse the
// state. Every other typed node is rejected rather than ignored.
BoundedSimulationResult simulate_bounded_quantum(const ResolvedHybridProgram& program,
                                                  const BoundedSimulationOptions& options);

}  // namespace synq::compiler

#endif
