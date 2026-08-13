// Copyright (c) 2025 SynQ Contributors

#include "gate_validation.h"

namespace synq::compiler {
namespace {

GateShapeValidationError non_parameterized_angle_error(const QuantumGateNode& gate) {
    return {"SYNQ-S001", "gate `" + gate.source_name + "` does not accept a literal angle",
            "remove the angle or use a documented parameterized gate such as rx(pi/2) q[0]"};
}

GateShapeValidationError arity_error(const QuantumGateNode& gate, const std::string& expectation) {
    return {"SYNQ-S002", "gate `" + gate.source_name + "` " + expectation,
            "match the documented explicit-qubit operand count for this gate"};
}

GateShapeValidationError parameterized_shape_error(const QuantumGateNode& gate) {
    return {"SYNQ-S003", "parameterized gate `" + gate.source_name + "` requires a literal angle and exactly one explicit qubit operand",
            "use a supported literal angle and one operand, such as rx(pi/2) q[0]"};
}

}  // namespace

std::optional<GateShapeValidationError> validate_quantum_gate_shape(const QuantumGateNode& gate) {
    switch (gate.kind) {
        case QuantumGateKind::H:
        case QuantumGateKind::X:
        case QuantumGateKind::Y:
        case QuantumGateKind::Z:
            if (gate.literal_angle.has_value()) return non_parameterized_angle_error(gate);
            if (gate.qubit_indices.size() > 1) return arity_error(gate, "accepts zero or one explicit qubit operand");
            return std::nullopt;
        case QuantumGateKind::Cx:
            if (gate.literal_angle.has_value()) return non_parameterized_angle_error(gate);
            if (gate.qubit_indices.size() != 2) return arity_error(gate, "requires exactly two explicit qubit operands");
            return std::nullopt;
        case QuantumGateKind::BellPair:
            if (gate.literal_angle.has_value()) return non_parameterized_angle_error(gate);
            if (gate.qubit_indices.size() != 0 && gate.qubit_indices.size() != 2) {
                return arity_error(gate, "accepts zero or exactly two explicit qubit operands");
            }
            return std::nullopt;
        case QuantumGateKind::Rx:
        case QuantumGateKind::Ry:
        case QuantumGateKind::Rz:
        case QuantumGateKind::Phase:
            if (!gate.literal_angle.has_value() || gate.qubit_indices.size() != 1) {
                return parameterized_shape_error(gate);
            }
            return std::nullopt;
        case QuantumGateKind::Unknown:
            return std::nullopt;
    }
    return std::nullopt;
}

}  // namespace synq::compiler
