// Minimal recovery-profile Hybrid IR.
// This internal representation preserves parsed immutable declarations, Alpha
// mutable cells/assignments, quantum gates, and measurements. It defines no
// target execution, allocation, ownership, result-value, or backend semantics.
#ifndef SYNQ_COMPILER_HYBRID_IR_H
#define SYNQ_COMPILER_HYBRID_IR_H

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include "classical_expression.h"
#include "diagnostic.h"

namespace synq::compiler {

struct HybridDeclaration {
    std::string name;
    std::string source_value;
    ClassicalLiteralKind literal_kind = ClassicalLiteralKind::SourceText;
    ClassicalExpression initializer;
    SourceSpan span;
};

struct HybridMutableDeclaration {
    std::string name;
    std::string source_value;
    ClassicalLiteralKind literal_kind = ClassicalLiteralKind::SourceText;
    ClassicalExpression initializer;
    SourceSpan span;
};

struct HybridAssignment {
    std::string target_name;
    std::string source_value;
    ClassicalLiteralKind literal_kind = ClassicalLiteralKind::SourceText;
    ClassicalExpression value;
    SourceSpan span;
};

struct HybridQubitDeclaration {
    std::string name;
    std::size_t qubit_count = 0;
    SourceSpan span;
};

struct HybridQuantumGate {
    QuantumGateKind kind = QuantumGateKind::Unknown;
    std::string source_name;
    std::optional<std::string> literal_angle;
    std::vector<std::size_t> qubit_indices;
    std::vector<std::string> qubit_register_names;
    SourceSpan span;
};

struct HybridRoutineFormal {
    RoutineFormalKind kind = RoutineFormalKind::Qubit;
    std::string name;
};

struct HybridParameterizedRoutineBody {
    QuantumGateKind kind = QuantumGateKind::Unknown;
    std::string source_name;
    std::optional<std::string> angle_formal;
    std::vector<std::string> qubit_formals;
    SourceSpan span;
};

struct HybridCallableDeclaration {
    CallableDeclarationKind kind = CallableDeclarationKind::Function;
    std::string name;
    std::optional<HybridQuantumGate> body;
    std::vector<HybridRoutineFormal> formals;
    std::optional<HybridParameterizedRoutineBody> parameterized_body;
    SourceSpan span;
};

struct HybridCallableCall {
    std::string name;
    std::vector<std::string> arguments;
    SourceSpan span;
};

struct HybridMeasurement {
    std::size_t qubit_index = 0;
    std::string qubit_register_name = "q";
    std::optional<std::string> result_name;
    SourceSpan span;
};

using HybridControlBody = std::variant<HybridQuantumGate, HybridMeasurement>;

struct HybridControlFlow {
    ClassicalControlKind kind = ClassicalControlKind::If;
    ClassicalCondition condition;
    HybridControlBody body;
    SourceSpan span;
};

using HybridNode = std::variant<HybridDeclaration, HybridMutableDeclaration, HybridAssignment,
                                HybridQubitDeclaration, HybridCallableDeclaration, HybridCallableCall,
                                HybridQuantumGate, HybridMeasurement, HybridControlFlow>;

struct HybridProgram {
    std::vector<HybridNode> nodes;
};

struct HybridLoweringResult {
    std::optional<HybridProgram> program;
    std::vector<Diagnostic> diagnostics;

    bool ok() const;
};

// Converts one successful recovery-parser ProgramNode into an internal Hybrid
// IR program. Unsupported legacy instruction nodes are rejected rather than
// silently dropped or assigned invented semantics.
HybridLoweringResult lower_to_hybrid_ir(const ProgramNode& program);

}  // namespace synq::compiler

#endif
