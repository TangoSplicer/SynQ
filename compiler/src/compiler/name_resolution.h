// Bounded recovery-profile scoped name resolution over the internal Hybrid IR.
// Only whole-identifier declaration initializers are references in this first
// pass. Other SourceText remains unevaluated and unresolved by design.
#ifndef SYNQ_COMPILER_NAME_RESOLUTION_H
#define SYNQ_COMPILER_NAME_RESOLUTION_H

#include <cstddef>
#include <optional>
#include <variant>
#include <vector>

#include "diagnostic.h"
#include "hybrid_ir.h"

namespace synq::compiler {

struct ResolvedHybridDeclaration {
    HybridDeclaration declaration;
    // Index of the earlier HybridProgram declaration referenced by a whole
    // identifier initializer. Absent means the initializer is a literal or
    // still-uninterpreted SourceText.
    std::optional<std::size_t> initializer_binding_index;
};

using ResolvedHybridNode = std::variant<ResolvedHybridDeclaration, HybridQuantumGate, HybridMeasurement>;

struct ResolvedHybridProgram {
    std::vector<ResolvedHybridNode> nodes;
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

}  // namespace synq::compiler

#endif
