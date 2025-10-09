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