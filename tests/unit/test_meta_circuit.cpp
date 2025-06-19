// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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