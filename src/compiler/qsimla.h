// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_QSIMLA_H
#define SYNQ_QSIMLA_H

#include <vector>
#include <complex>
#include <string>
#include <optional>

namespace synq {

    using Complex = std::complex<double>;

    // Latent state snapshot from simulation
    struct LatentState {
        std::vector<Complex> amplitudes;
        std::optional<std::vector<double>> latent_vector;
        std::string circuit_hash;
    };

    // QSim-LA: Quantum simulation engine with latent alignment
    class QSimLA {
    public:
        QSimLA(int num_qubits);

        void apply_gate(const std::string& gate, const std::vector<int>& qubits, std::optional<double> param = std::nullopt);
        LatentState snapshot() const;

        void inject_latent_vector(const std::vector<double>& vec);
        double compare_alignment(const LatentState& a, const LatentState& b) const;

        void reset();

    private:
        int qubits_;
        std::vector<Complex> state_;
        std::optional<std::vector<double>> current_latent_;
        std::string compute_hash() const;

        void apply_unitary(const std::string& gate, int target, std::optional<double> param);
    };

} // namespace synq

#endif  // SYNQ_QSIMLA_H