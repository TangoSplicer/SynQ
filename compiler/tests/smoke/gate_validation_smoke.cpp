#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "compiler/gate_validation.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool has_error(const QuantumGateNode& gate, const std::string& code) {
    const std::optional<synq::compiler::GateShapeValidationError> error =
        synq::compiler::validate_quantum_gate_shape(gate);
    return error.has_value() && error->code == code && !error->message.empty() && !error->help.empty();
}

bool validates_known_gate_shapes() {
    const QuantumGateNode h_ok(QuantumGateKind::H, "h", std::nullopt, {0}, 1);
    const QuantumGateNode cx_ok(QuantumGateKind::Cx, "cx", std::nullopt, {0, 1}, 2);
    const QuantumGateNode bell_ok(QuantumGateKind::BellPair, "bell_pair", std::nullopt, {}, 3);
    const QuantumGateNode rx_ok(QuantumGateKind::Rx, "rx", std::string("pi/2"), {2}, 4);
    const QuantumGateNode h_angle(QuantumGateKind::H, "h", std::string("pi/2"), {0}, 5);
    const QuantumGateNode cx_arity(QuantumGateKind::Cx, "cx", std::nullopt, {0}, 6);
    const QuantumGateNode bell_arity(QuantumGateKind::BellPair, "bell_pair", std::nullopt, {0}, 7);
    const QuantumGateNode rx_shape(QuantumGateKind::Rx, "rx", std::nullopt, {0}, 8);

    return require(!synq::compiler::validate_quantum_gate_shape(h_ok).has_value(), "H accepts one operand") &&
           require(!synq::compiler::validate_quantum_gate_shape(cx_ok).has_value(), "Cx accepts two operands") &&
           require(!synq::compiler::validate_quantum_gate_shape(bell_ok).has_value(), "BellPair accepts no operands") &&
           require(!synq::compiler::validate_quantum_gate_shape(rx_ok).has_value(), "Rx accepts literal angle and one operand") &&
           require(has_error(h_angle, "SYNQ-S001"), "non-parameterized angle is SYNQ-S001") &&
           require(has_error(cx_arity, "SYNQ-S002"), "known gate arity is SYNQ-S002") &&
           require(has_error(bell_arity, "SYNQ-S002"), "bell-pair arity is SYNQ-S002") &&
           require(has_error(rx_shape, "SYNQ-S003"), "parameterized gate shape is SYNQ-S003");
}

bool preserves_unknown_gates() {
    const QuantumGateNode unknown(QuantumGateKind::Unknown, "qaoa", std::string("pi/2"), {0, 1}, 9);
    return require(!synq::compiler::validate_quantum_gate_shape(unknown).has_value(),
                   "unknown gate names remain for a selected backend to diagnose");
}

}  // namespace

int main() {
    if (!validates_known_gate_shapes()) return 1;
    if (!preserves_unknown_gates()) return 1;
    std::cout << "SynQ gate-shape validation smoke test passed\n";
    return 0;
}
