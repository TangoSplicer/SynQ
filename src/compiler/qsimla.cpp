// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "qsimla.h"
#include <cmath>
#include <stdexcept>
#include <sstream>
#include <random>
#include <numeric>

namespace synq {

    QSimLA::QSimLA(int num_qubits) : qubits_(num_qubits) {
        size_t dim = 1ULL << num_qubits;
        state_.resize(dim, Complex(0.0, 0.0));
        state_[0] = Complex(1.0, 0.0);  // |0...0>
    }

    void QSimLA::apply_gate(const std::string& gate, const std::vector<int>& qubits, std::optional<double> param) {
        if (qubits.empty()) throw std::invalid_argument("No target qubits provided");
        apply_unitary(gate, qubits[0], param);  // basic single-qubit placeholder
    }

    void QSimLA::apply_unitary(const std::string& gate, int target, std::optional<double> param) {
        // Basic gate emulation
        double theta = param.value_or(M_PI / 4);
        for (size_t i = 0; i < state_.size(); ++i) {
            if (((i >> target) & 1) == 1) {
                state_[i] *= std::polar(1.0, theta);  // Rotate phase
            }
        }
    }

    LatentState QSimLA::snapshot() const {
        LatentState snap;
        snap.amplitudes = state_;
        snap.latent_vector = current_latent_;
        snap.circuit_hash = compute_hash();
        return snap;
    }

    void QSimLA::inject_latent_vector(const std::vector<double>& vec) {
        current_latent_ = vec;
    }

    double QSimLA::compare_alignment(const LatentState& a, const LatentState& b) const {
        if (!a.latent_vector || !b.latent_vector) return -1.0;

        const auto& va = a.latent_vector.value();
        const auto& vb = b.latent_vector.value();

        if (va.size() != vb.size()) return -2.0;

        double dot = std::inner_product(va.begin(), va.end(), vb.begin(), 0.0);
        double na = std::sqrt(std::inner_product(va.begin(), va.end(), va.begin(), 0.0));
        double nb = std::sqrt(std::inner_product(vb.begin(), vb.end(), vb.begin(), 0.0));
        return dot / (na * nb);  // Cosine similarity
    }

    void QSimLA::reset() {
        std::fill(state_.begin(), state_.end(), Complex(0.0));
        state_[0] = Complex(1.0, 0.0);
        current_latent_.reset();
    }

    std::string QSimLA::compute_hash() const {
        std::ostringstream oss;
        for (const auto& amp : state_) {
            oss << amp.real() << ":" << amp.imag() << ";";
        }
        return std::to_string(std::hash<std::string>{}(oss.str()));
    }

} // namespace synq