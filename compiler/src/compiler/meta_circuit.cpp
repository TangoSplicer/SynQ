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