// Minimal recovery-profile Hybrid IR lowering implementation.
#include "hybrid_ir.h"

#include <utility>

namespace synq::compiler {

bool HybridLoweringResult::ok() const {
    if (!program.has_value()) return false;
    for (const Diagnostic& diagnostic : diagnostics) {
        if (diagnostic.severity == DiagnosticSeverity::Error) return false;
    }
    return true;
}

namespace {

Diagnostic unsupported_node_diagnostic(const ASTNode* node) {
    SourceSpan span;
    std::string name = "unknown AST node";

    if (const auto* instruction = dynamic_cast<const InstructionNode*>(node)) {
        span = instruction->span;
        name = "legacy instruction `" + instruction->op + "`";
    }

    return {
        "SYNQ-H001",
        DiagnosticSeverity::Error,
        span,
        "Hybrid IR does not yet represent " + name,
        "use only let declarations, typed quantum gates, and measurements until this internal IR expands"
    };
}

Diagnostic unsupported_control_body_diagnostic(const ClassicalControlNode& control) {
    return {
        "SYNQ-H002",
        DiagnosticSeverity::Error,
        control.span,
        "Hybrid IR control-flow body is not a typed quantum gate or measurement",
        "use the parser-produced bounded control-flow nodes until internal control regions expand"
    };
}

}  // namespace

HybridLoweringResult lower_to_hybrid_ir(const ProgramNode& program) {
    HybridProgram lowered;
    lowered.nodes.reserve(program.statements.size());

    for (const ASTNode* statement : program.statements) {
        if (const auto* declaration = dynamic_cast<const DeclarationNode*>(statement)) {
            lowered.nodes.emplace_back(HybridDeclaration{
                declaration->name,
                declaration->value,
                declaration->literal_kind,
                make_classical_expression(declaration->value, declaration->literal_kind, declaration->span),
                declaration->span,
            });
            continue;
        }

        if (const auto* declaration = dynamic_cast<const MutableDeclarationNode*>(statement)) {
            lowered.nodes.emplace_back(HybridMutableDeclaration{
                declaration->name,
                declaration->value,
                declaration->literal_kind,
                make_classical_expression(declaration->value, declaration->literal_kind, declaration->span),
                declaration->span,
            });
            continue;
        }

        if (const auto* assignment = dynamic_cast<const AssignmentNode*>(statement)) {
            lowered.nodes.emplace_back(HybridAssignment{
                assignment->target,
                assignment->value,
                assignment->literal_kind,
                make_classical_expression(assignment->value, assignment->literal_kind, assignment->span),
                assignment->span,
            });
            continue;
        }

        if (const auto* qubits = dynamic_cast<const QubitDeclarationNode*>(statement)) {
            lowered.nodes.emplace_back(HybridQubitDeclaration{
                qubits->name,
                qubits->qubit_count,
                qubits->span,
            });
            continue;
        }

        if (const auto* callable = dynamic_cast<const CallableDeclarationNode*>(statement)) {
            std::optional<HybridQuantumGate> body;
            if (callable->body != nullptr) {
                body.emplace(HybridQuantumGate{callable->body->kind, callable->body->source_name,
                                                callable->body->literal_angle, callable->body->qubit_indices,
                                                callable->body->qubit_register_names, callable->body->span});
            }
            lowered.nodes.emplace_back(HybridCallableDeclaration{
                callable->kind,
                callable->name,
                std::move(body),
                callable->span,
            });
            continue;
        }

        if (const auto* call = dynamic_cast<const CallableCallNode*>(statement)) {
            lowered.nodes.emplace_back(HybridCallableCall{call->name, call->span});
            continue;
        }

        if (const auto* gate = dynamic_cast<const QuantumGateNode*>(statement)) {
            lowered.nodes.emplace_back(HybridQuantumGate{
                gate->kind,
                gate->source_name,
                gate->literal_angle,
                gate->qubit_indices,
                gate->qubit_register_names,
                gate->span,
            });
            continue;
        }

        if (const auto* measurement = dynamic_cast<const MeasurementNode*>(statement)) {
            lowered.nodes.emplace_back(HybridMeasurement{
                measurement->qubit_index,
                measurement->qubit_register_name,
                measurement->result_name,
                measurement->span,
            });
            continue;
        }

        if (const auto* control = dynamic_cast<const ClassicalControlNode*>(statement)) {
            if (const auto* gate = dynamic_cast<const QuantumGateNode*>(control->body)) {
                lowered.nodes.emplace_back(HybridControlFlow{
                    control->kind,
                    control->condition,
                    HybridQuantumGate{gate->kind, gate->source_name, gate->literal_angle, gate->qubit_indices,
                                      gate->qubit_register_names, gate->span},
                    control->span,
                });
                continue;
            }
            if (const auto* measurement = dynamic_cast<const MeasurementNode*>(control->body)) {
                lowered.nodes.emplace_back(HybridControlFlow{
                    control->kind,
                    control->condition,
                    HybridMeasurement{measurement->qubit_index, measurement->qubit_register_name,
                                      measurement->result_name, measurement->span},
                    control->span,
                });
                continue;
            }
            HybridLoweringResult result;
            result.diagnostics.push_back(unsupported_control_body_diagnostic(*control));
            return result;
        }

        HybridLoweringResult result;
        result.diagnostics.push_back(unsupported_node_diagnostic(statement));
        return result;
    }

    HybridLoweringResult result;
    result.program = std::move(lowered);
    return result;
}

}  // namespace synq::compiler
