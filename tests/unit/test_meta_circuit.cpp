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
#include "compiler/meta_circuit.h"
#include <cassert>
#include <iostream>

using namespace synq;

void test_add_and_stringify() {
    MetaCircuit mc;
    mc.add_op({"H", {0}, std::nullopt});
    mc.add_op({"CNOT", {0, 1}, std::nullopt});
    std::string rep = mc.to_string();
    assert(rep.find("H(0)") != std::string::npos);
    assert(rep.find("CNOT(0,1)") != std::string::npos);
    std::cout << "test_add_and_stringify passed.\n";
}

void test_merge() {
    MetaCircuit a, b;
    a.add_op({"H", {0}, std::nullopt});
    b.add_op({"X", {1}, std::nullopt});
    a.merge(b);
    assert(a.get_ops().size() == 2);
    std::cout << "test_merge passed.\n";
}

void test_invert() {
    MetaCircuit mc;
    mc.add_op({"A", {0}, std::nullopt});
    mc.add_op({"B", {1}, std::nullopt});
    mc.invert();
    auto ops = mc.get_ops();
    assert(ops[0].gate == "B");
    std::cout << "test_invert passed.\n";
}

int main() {
    test_add_and_stringify();
    test_merge();
    test_invert();
    return 0;
}