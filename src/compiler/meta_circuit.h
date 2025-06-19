// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_META_CIRCUIT_H
#define SYNQ_META_CIRCUIT_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <optional>

namespace synq {

    // Abstract representation of a circuit operation
    struct MetaOp {
        std::string gate;       // e.g., "H", "CNOT"
        std::vector<int> qubits;
        std::optional<double> param;  // for rotations, phases
    };

    // Symbolic meta-circuit structure
    class MetaCircuit {
    public:
        MetaCircuit();

        void add_op(const MetaOp& op);
        void remove_op(size_t index);
        void invert();
        void merge(const MetaCircuit& other);
        void mutate();

        std::vector<MetaOp> get_ops() const;
        std::string to_string() const;

    private:
        std::vector<MetaOp> ops_;
    };

}  // namespace synq

#endif  // SYNQ_META_CIRCUIT_H