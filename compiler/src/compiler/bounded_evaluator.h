// Deterministic bounded constant evaluation for a deliberately tiny subset of
// resolved declarations. This is not a general runtime or quantum executor.
#ifndef SYNQ_COMPILER_BOUNDED_EVALUATOR_H
#define SYNQ_COMPILER_BOUNDED_EVALUATOR_H

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "diagnostic.h"
#include "name_resolution.h"

namespace synq::compiler {

enum class BoundedValueKind {
    Integer,
    Boolean,
    String,
};

struct BoundedValue {
    BoundedValueKind kind = BoundedValueKind::Integer;
    std::int64_t integer_value = 0;
    bool boolean_value = false;
    std::string string_value;
};

struct EvaluatedBinding {
    std::string name;
    BoundedValue value;
    SourceSpan span;
};

struct BoundedEvaluation {
    std::vector<EvaluatedBinding> bindings;
};

struct BoundedEvaluationOptions {
    // The caller must explicitly opt in. This prevents accidental interpretation
    // of a parsed program as an executable program.
    bool allow_experimental_constant_evaluation = false;
    std::size_t max_declarations = 64;
};

struct BoundedEvaluationResult {
    std::optional<BoundedEvaluation> evaluation;
    std::vector<Diagnostic> diagnostics;

    bool ok() const;
};

// Evaluates only a sequence of resolved top-level declarations with supported
// literal, alias, or one-operator Integer arithmetic initializers. Qubits,
// measurements, gates, controls, callables, decimals, opaque source, I/O,
// mutable state, loops, calls, and quantum execution are rejected explicitly.
BoundedEvaluationResult evaluate_bounded_constants(const ResolvedHybridProgram& program,
                                                   const BoundedEvaluationOptions& options);

const char* bounded_value_kind_name(BoundedValueKind kind);

}  // namespace synq::compiler

#endif
