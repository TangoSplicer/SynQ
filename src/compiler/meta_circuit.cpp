// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "meta_circuit.h"
#include <sstream>
#include <random>
#include <algorithm>

namespace synq {

    MetaCircuit::MetaCircuit() {}

    void MetaCircuit::add_op(const MetaOp& op) {
        ops_.push_back(op);
    }

    void MetaCircuit::remove_op(size_t index) {
        if (index < ops_.size()) {
            ops_.erase(ops_.begin() + index);
        }
    }

    void MetaCircuit::invert() {
        std::reverse(ops_.begin(), ops_.end());
    }

    void MetaCircuit::merge(const MetaCircuit& other) {
        for (const auto& op : other.get_ops()) {
            ops_.push_back(op);
        }
    }

    void MetaCircuit::mutate() {
        if (ops_.empty()) return;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> action(0, 2);
        std::uniform_int_distribution<> index(0, ops_.size() - 1);

        int choice = action(gen);
        if (choice == 0) { // add random op
            MetaOp op = {"X", {0}, std::nullopt};
            ops_.insert(ops_.begin() + index(gen), op);
        } else if (choice == 1) { // remove
            ops_.erase(ops_.begin() + index(gen));
        } else { // invert gate name
            size_t i = index(gen);
            ops_[i].gate += "'";
        }
    }

    std::vector<MetaOp> MetaCircuit::get_ops() const {
        return ops_;
    }

    std::string MetaCircuit::to_string() const {
        std::ostringstream oss;
        for (const auto& op : ops_) {
            oss << op.gate << "(";
            for (size_t i = 0; i < op.qubits.size(); ++i) {
                oss << op.qubits[i];
                if (i < op.qubits.size() - 1) oss << ",";
            }
            oss << ")";
            if (op.param.has_value()) {
                oss << "[" << op.param.value() << "]";
            }
            oss << "\n";
        }
        return oss.str();
    }

} // namespace synq