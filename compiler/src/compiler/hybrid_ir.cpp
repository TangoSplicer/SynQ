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
                declaration->span,
            });
            continue;
        }

        if (const auto* gate = dynamic_cast<const QuantumGateNode*>(statement)) {
            lowered.nodes.emplace_back(HybridQuantumGate{
                gate->kind,
                gate->source_name,
                gate->literal_angle,
                gate->qubit_indices,
                gate->span,
            });
            continue;
        }

        if (const auto* measurement = dynamic_cast<const MeasurementNode*>(statement)) {
            lowered.nodes.emplace_back(HybridMeasurement{
                measurement->qubit_index,
                measurement->span,
            });
            continue;
        }

        if (const auto* control = dynamic_cast<const ClassicalControlNode*>(statement)) {
            if (const auto* gate = dynamic_cast<const QuantumGateNode*>(control->body)) {
                lowered.nodes.emplace_back(HybridControlFlow{
                    control->kind,
                    control->condition,
                    HybridQuantumGate{gate->kind, gate->source_name, gate->literal_angle, gate->qubit_indices, gate->span},
                    control->span,
                });
                continue;
            }
            if (const auto* measurement = dynamic_cast<const MeasurementNode*>(control->body)) {
                lowered.nodes.emplace_back(HybridControlFlow{
                    control->kind,
                    control->condition,
                    HybridMeasurement{measurement->qubit_index, measurement->span},
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
