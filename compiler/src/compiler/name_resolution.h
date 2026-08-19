// Bounded recovery-profile scoped name resolution over the internal Hybrid IR.
// Whole-identifier initializers plus bounded parser-owned Boolean/Integer trees
// resolve against prior top-level bindings. Other SourceText remains
// unevaluated and unresolved by design.
#ifndef SYNQ_COMPILER_NAME_RESOLUTION_H
#define SYNQ_COMPILER_NAME_RESOLUTION_H

#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

#include "diagnostic.h"
#include "hybrid_ir.h"

namespace synq::compiler {

enum class SemanticBindingKind {
    Value,
    MutableCell,
    MeasurementResult,
};

const char* semantic_binding_kind_name(SemanticBindingKind kind);

// A read-only record of one resolved top-level classical binding. It describes
// identity and static type only; it does not contain a runtime value or effect.
struct SemanticBinding {
    std::string name;
    SemanticBindingKind kind = SemanticBindingKind::Value;
    ClassicalStaticType static_type = ClassicalStaticType::Unknown;
    std::size_t node_index = 0;
    SourceSpan span;
    std::vector<std::string> dependency_names;
};

struct ResolvedHybridDeclaration {
    HybridDeclaration declaration;
    // Index of the earlier HybridProgram declaration referenced by a whole
    // identifier initializer. Absent means the initializer is a literal or
    // still-uninterpreted SourceText.
    std::optional<std::size_t> initializer_binding_index;
    ClassicalStaticType initializer_static_type = ClassicalStaticType::Unknown;
    // Indices of earlier declaration operands used by a bounded integer
    // arithmetic initializer. Empty means no identifier arithmetic operands.
    std::vector<std::size_t> initializer_binding_indices;
};

struct ResolvedHybridMutableDeclaration {
    HybridMutableDeclaration declaration;
    std::optional<std::size_t> initializer_binding_index;
    ClassicalStaticType initializer_static_type = ClassicalStaticType::Unknown;
    std::vector<std::size_t> initializer_binding_indices;
};

struct ResolvedHybridAssignment {
    HybridAssignment assignment;
    std::size_t target_binding_index = 0;
    ClassicalStaticType target_static_type = ClassicalStaticType::Unknown;
    std::optional<std::size_t> value_binding_index;
    ClassicalStaticType value_static_type = ClassicalStaticType::Unknown;
    std::vector<std::size_t> value_binding_indices;
};

struct ResolvedHybridControlFlow {
    HybridControlFlow control;
    std::optional<std::size_t> condition_binding_index;
    std::vector<std::size_t> condition_binding_indices;
};

// U4's sole hybrid feedback form joins one named measurement with its immediate
// direct conditional X correction. It records provenance and static ordering;
// it is not a general control-flow or measurement execution node.
struct ResolvedHybridMeasurementFeedback {
    HybridMeasurement measurement;
    HybridControlFlow correction;
    std::size_t result_binding_index = 0;
};

using ResolvedHybridNode = std::variant<ResolvedHybridDeclaration, ResolvedHybridMutableDeclaration,
                                        ResolvedHybridAssignment, HybridQubitDeclaration,
                                        HybridCallableDeclaration, HybridCallableCall, HybridQuantumGate,
                                        HybridMeasurement, ResolvedHybridControlFlow,
                                        ResolvedHybridMeasurementFeedback>;

struct ResolvedHybridProgram {
    std::vector<ResolvedHybridNode> nodes;
    std::vector<SemanticBinding> semantic_bindings;
};

struct NameResolutionResult {
    std::optional<ResolvedHybridProgram> program;
    std::vector<Diagnostic> diagnostics;

    bool ok() const;
};

// Resolves declaration initializers against prior declarations in one top-level
// HybridProgram scope. It does not evaluate expressions, inspect strings,
// create nested scopes, or assign types.
NameResolutionResult resolve_hybrid_names(const HybridProgram& program);

// Renders the resolved top-level semantic environment for inspection. The output
// intentionally reports static types and dependency names, not evaluated values.
std::string render_semantic_environment(const ResolvedHybridProgram& program);

}  // namespace synq::compiler

#endif
