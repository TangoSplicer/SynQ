// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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